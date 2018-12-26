#include "GPRSLib.h"
#include "gprs.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "SHT10.h"
#include "BMP280.h"

struct retstruct
{
    u8 ret1;
    u8 ret2;
}r_stRetData;

extern u8 GPRSInitFinishFlag;
extern void PDAOperationUpload(void);
extern u8 GPRSCMDNum;
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);  
extern void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
extern uint16_t CollectionCount;
extern uint16_t Sensor_UploadCycle;             //传感器上传周期
//气压
extern float BMP280_Pressure;
//海拔
extern float BMP280_High;
//温度
extern float BMP280_Tem;
extern uint8_t CollectionFinishFlag;
extern uint16_t GPRS_DataSendTim;
extern uint16_t SensorCycle;
extern uint8_t USART2_UartReceiving;
extern uint8_t CompTimeFlag;                       //授时完成标志位
extern void SensorDataWrite2Buf(uint8_t NetMode);
extern void PDAMemoryDataWrite2Buf(uint8_t NetMode);
extern uint8_t NeedRBTFlag;
extern void LockDataAck(uint8_t NetMode);
extern uint8_t USART2_UartDataFinishFlag;
extern void SendLockOptionData(uint8_t NetMode);
extern uint8_t ClockFlag;
extern void ICDataACK(uint8_t NetMode);
extern uint8_t ICFlag;
extern void SensorTimConACK(uint8_t NetMode,uint8_t *GPRS_RxDataBuf);
extern uint8_t SensorTimeConfigFlag;
extern void ASICC2Char(uint8_t *Sou,uint8_t *Des,uint8_t Length);
extern void GPSTimConACK(uint8_t NetMode,uint8_t *GPRS_RxDataBuf);
extern uint8_t GPSTimeConfigFlag;
extern void RTCDataACK(uint8_t NetMode);
extern uint8_t RTCTimeFlag;
extern uint8_t NeedACK;
extern uint8_t WaitingACKCount;
extern uint8_t TernimalACKCount;
extern uint8_t ThereHaveSM;
extern uint8_t TernimalACKTimCount;
extern uint8_t ConnectSurvival;
extern uint8_t GPRS_RxDataBuf[125];
extern void NetRxDataDeal(uint8_t *IOT_ReceiveBuf);
extern uint8_t CloseLedORScreeTim;
extern uint8_t NeedFrashScreeNetFlag;
extern uint8_t NeedCloseLedFlag;
extern void LCD12864_wstr(unsigned char y_add , unsigned char x_add , unsigned char *str);
extern uint8_t NetJoinFlag;
extern uint8_t ContrastData(uint8_t *string);                                     //对比数据
extern u8 GPRS_Rssi;
extern u8 GPRSCSQOK;
extern uint8_t ExpectReceDataCSQ(uint8_t *string, struct retstruct* ret);
extern u8 GPRSAckSendOK;
extern u8 GPRSAckWrite;
extern uint8_t ReLength;
extern uint8_t GetServiceData(uint8_t *string,uint8_t *ReBuf);

u8 GPRSState = 0;

void GPRS_Start(void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_4);
  delay_s(1);
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void GPRS_OFF(void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_4);
  delay_s(1);
  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

void GPRS_GpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
  
  /*GPRS Power*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPRS_U2Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* 配置串口2 Tx（PA2）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 Rx（PA3）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 的各种参数 */
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  /* 串口2开始工作 */
  USART_Cmd(USART2, ENABLE);
  
  USART_ClearFlag(USART2,USART_FLAG_TC);
}

void GPRS_DeviceInit(void)
{
  GPRS_GpioInit();
  GPRS_U2Init();
}

void GPRS_AutoInit(void)
{
  uint8_t num = 0;
  switch(GPRSCMDNum)
  {
    case 0:
    {
      if(GPRS_AT_Test())                                                        //AT测试命令
        GPRSCMDNum++;
      break;
    }
    case 1:
    {
      if(GPRS_SetATE())                                                        //关闭回显命令
      {
        AT24CXX_Read(GPRS_YorNFirst,&GPRSState,1);                                 //读取GPRS状态
        if(GPRSState == 1)
        {
          GPRSCMDNum++;
          GPRSState++;
          AT24CXX_Write(GPRS_YorNFirst,&GPRSState,1);
        }
        else
          GPRSCMDNum += 2;
      }
      break;
    }
    case 2:                                                                     //设置固定波特率
    {
      if(GPRS_SetBT())
        GPRSCMDNum++;
      break;
    }
    case 3:                                                                     //确保SIM的PIN码已经解析
    {
      if(GPRS_SIMRight())
        GPRSCMDNum++;
      break;
    }  
    case 4:                                                                     //上报移动设备错误
    {
      if(GPRS_UpLoadError())
        GPRSCMDNum++;
      break;
    }
    case 5:                                                                     //启用网络注册和位置信息非请求结果码
    {
      if(GPRS_CREGSET())
        GPRSCMDNum++;
      break;
    }
    case 6:                                                                     //确定找网成功
    {
      if(GPRS_FindNet())
        GPRSCMDNum++;
      break;
    }   
 
    case 7:                                                                     //查询GPRS附着是否成功  网络注册状态
    {
      if(GPRS_Check())
        GPRSCMDNum++;
      break;
    }//////////////////////////////////////////////以下为连接网络 
    case 8:
    {
      num = GPRS_QIMUXCheck();
      if(num == 1)                                                           //多连接检查
        GPRSCMDNum += 2;
      else if(num == 2)
      {
        GPRSCMDNum++;
      }
      break;
    }
    case 9:
    {
      if(GPRS_QIMUX())                                                           //建立多连接
        GPRSCMDNum++;
      break;
    }
    case 10:                                                                     //关闭除IP INITIAL状态的任何GPRS场景
    {
      num = GPRS_QIDEACT();
      if(num == 1)
        GPRSCMDNum++;
      else if(num == 2)                                             //关闭场景失败
        GPRSCMDNum = 3;
      break;
    }
    case 11:
    {
      if(GPRS_QIREGAPP())                                                       //启动任务并设置接入点 APN、用户名和密码  账号密码不用加。
        GPRSCMDNum++;
      break;
    }
    case 12:
    {
      num = GPRS_QIACT();
      if(num == 1)                                                     //发起GPRS无线连接  激活场景
        GPRSCMDNum++;
      else if(num == 2)                                               //激活场景失败
        GPRSCMDNum = 7;
      break;
    }
    case 13:
    {
      num = GPRS_QISTAT();
      switch(num)                                                       //查询连接状态
      {
        case 1:                                                         //命令返回正常
        {
          GPRSCMDNum++;
          break;
        }
        case 2:                                                         //TCP已建立
        {
          GPRSCMDNum += 2;
          break;
        }
        case 3:                                                         //UDP已建立
        {
          GPRSCMDNum += 3;
          break;
        }
        default:
          break;
      }
        
      break;
    }
    case 14:
    {
      num = GPRS_QIOPRN_TCP();
      if(num == 1)                                                //建立TCP连接
        GPRSCMDNum++;
      else if(num == 2)                                               //建立连接失败
        GPRSCMDNum = 13;
      break;
    }
    case 15:
    {
      num = GPRS_QIOPRN_UDP();
      if(num == 1)                                                     //建立UDP连接
        GPRSCMDNum++;
      else if(num == 2)                                               //激活场景失败
        GPRSCMDNum = 13;
      break;
    } 
    case 16:
    {
      if(GPRS_QINDISet())                                                       //设置数据接收不缓存 直接输出至串口
        GPRSCMDNum++;
      break;
    }
    case 17:                                                                    //建立连接成功成功
    {
      GPRSCMDNum = 0;
      GPRSInitFinishFlag = 1;                                                //初始化完成 
    }
    default:   
      break;
  }
}

void GPRS_Init(void)
{
  if(!GPRSInitFinishFlag)
  {
    if(GPRSTimCount >= 50)                                                     //250ms
    {
      GPRSTimCount = 0;
      GPRS_AutoInit();                                                          //GPRS自动连接网络
    }
  }
  USART2_ReceiveData();                                                         //GPRS接收数据
}

void SensorCycleCollect(void)
{
  if(CollectionCount >= Sensor_UploadCycle*5)                                          //采集周期    
  {
    CollectionCount = 0;
    read_temperature_and_humidity();
    bmp280GetData(&BMP280_Pressure,&BMP280_Tem,&BMP280_High);                                                //读气压
    CollectionFinishFlag = 1;
  }
}

/*发送数据*/
void GPRS_CollectionAndSendData(void)
{  
  SensorCycleCollect();
  
  if(GPRS_DataSendTim >= SensorCycle && CollectionFinishFlag && !USART2_UartReceiving && CompTimeFlag)                             //采集数据发送                          //100ms
  {
    GPRS_DataSendTim = 0;  
    
    SensorDataWrite2Buf(GPRS);                                                  //将采集到的数据通过参数方式发送出去
    
    PDAMemoryDataWrite2Buf(GPRS);                                               //发送存储PDA的数据
  }
}

uint8_t GPRSHaveMsgFlag = 0;                            //GPRS有数据标志

void GPRS_HaveReceive(void)                             //入网成功后的所有串口数据检测
{
  uint8_t test[255] = {0};
  if(USART2_UartDataFinishFlag)
  { 
    while(1)
    {
      if(GetServiceData("\r\n+RECEIVE: 0, ",GPRS_RxDataBuf))
      {
        ASICC2Char(GPRS_RxDataBuf,test,ReLength);                                                  //方便串口观察数据
      
        Usart_SendNumString(USART1,test,2*ReLength);
        
        USART2_UartDataFinishFlag = 0;
        GPRSHaveMsgFlag = 1;
      }
      else
        break;                                                                  //防止出现平台连续发两条的现象
    }
    if(ContrastData("\r\n> "))                                               //成功
    {
      GPRSAckWrite = 1;
      USART2_UartDataFinishFlag = 0;
    }
    if(ContrastData("\r\nSEND OK\r\n"))
    {    
      GPRSAckSendOK = 1;
      USART2_UartDataFinishFlag = 0;
    }
    if(ExpectReceDataCSQ("\r\n+CSQ: ",&r_stRetData) == 0)
    {
      GPRSCSQOK = 1;
      GPRS_Rssi = r_stRetData.ret1;                                             //提取信号强度
      USART2_UartDataFinishFlag = 0;
    }
    if(ContrastData("CLOSED"))                                                  //TCP连接断开
    {
      GPRSInitFinishFlag = 0;                                       
      GPRSCMDNum = 10;                                               //重新回到初始化
      ConnectSurvival = 0;
      NetJoinFlag = 0;                                               //网络连接状态断开
      
      LCD12864_wstr(2,0,"GPRS网络连接断开");
      NeedFrashScreeNetFlag = 1;
      CloseLedORScreeTim = 0;
      LCD12864_wstr(4,2," OFF");
    }
  }
}

/*接收数据*/
void GPRS_ReceiveData(void)
{
    GPRS_HaveReceive();
    if(GPRSHaveMsgFlag)                                  //收到数据  收数据的优先级高
    {         
        GPRSHaveMsgFlag = 0;
        ThereHaveSM = 1;
        
        LED_BLUE_H();                                                           //收到NB数据
        NeedCloseLedFlag = 1;
        CloseLedORScreeTim = 0;
        
        NetRxDataDeal(GPRS_RxDataBuf);                                          //数据指令的执行  具体看"网络通信协议"
    } 
    
    else if(TernimalACKTimCount > 10 && ThereHaveSM)                           //50ms 执行操作的回复 具体看"网络通信协议"
    {
        TernimalACKTimCount = 0;
        TernimalACKCount++;
        WaitingACKCount++;
        
        if(NeedACK == 0x01)                                                       //需要ACK
        {
          TernimalACKCount = 0;
          if(RTCTimeFlag)                                                             //对RTC时钟进行修改
          {    
            RTCDataACK(GPRS);
          }
          else if(GPSTimeConfigFlag)                                                  //设置GPS上传周期
          {
            GPSTimConACK(GPRS,GPRS_RxDataBuf);
          }
          else if(SensorTimeConfigFlag)                                             //设置传感器采集周期
          {
            SensorTimConACK(GPRS,GPRS_RxDataBuf);
          }
          else if(ICFlag)                                                       //IC授权
          {
            ICDataACK(GPRS);
          }
          else if(ClockFlag)                                                    //电子锁操作
          {
            SendLockOptionData(GPRS);
            
            LockDataAck(GPRS);                                                  //锁操作的回码
          }
       }
        
       if(TernimalACKCount > 3)
       {
          TernimalACKCount = 0;
          NeedRBTFlag = 0;
       }
    }
}

void GPRS_RxTxData(void)
{
  if(GPRSInitFinishFlag)
  {
#ifdef SensorEquip
    if(NetJoinFlag == 2)                                                        //入网成功
    {
      GPRS_CollectionAndSendData();                                                            //数据采集和发送
    }
#endif
    GPRS_ReceiveData();                                                         //数据接收与处理
    PDAOperationUpload();                                                       //PDA操作上传
  }
}

void GPRSOption(void)
{
  if(GPRS)   
  {
    GPRS_Init();                                                                //GPRS初始化，包括入网初始化
    
    GPRS_RxTxData();                                                            //GPRS发送和接收数据函数
  }
}