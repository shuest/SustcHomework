#include "NBIOTLib.h"
#include "NBIOT.h"
#include "usart.h"
#include "gpio.h"

extern uint8_t NetJoinFlag;
extern uint8_t NBIOT;
extern void USART3_ReceiveData(void);   /*判断是都接收完成函数*/
extern uint8_t USART3_UartDataFinishFlag;
extern uint8_t NB_RxFlag;
extern uint8_t ExpectReceDataCEREG(uint8_t *string, uint8_t *ret);
extern uint8_t Rec_length;
extern uint8_t NB_RxDataCount;
extern uint8_t NB_CycReadTim;
extern uint8_t NSORF_ComFlag;
extern uint8_t NB_ContrastData(uint8_t *string);
extern uint8_t NB_ReceOKFlag;
extern uint8_t CSQFlag;
extern uint8_t NSOST_ComFlag;
extern uint16_t NB_DataSendTim;
extern uint8_t CEREG_Flag;
extern uint16_t SensorCycle;
extern uint8_t CloseGPRSTim;
extern uint8_t CloseGPRSFlag;
extern void GPRS_Close(void);
extern uint8_t NBScramblingState;
extern uint8_t NBInitFinishFlag;
extern uint8_t NBTimCount;  
extern int UDPPort;                             //UDP端口号
extern void NB_CycleReadBuf(void);
extern void SensorCycleCollect(void);
extern void PDAMemoryDataWrite2Buf(uint8_t NetMode);
extern void SensorDataWrite2Buf(uint8_t NetMode);
extern uint8_t CollectionFinishFlag;
extern uint8_t NB_ReceiveFirst;
extern uint8_t CompTimeFlag;                       //授时完成标志位

uint8_t NB_NetSign = 0;
uint8_t CMDSuccess = 0;

void NB_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  
  
  /* 配置NB POWER为推挽输出模式 BLUE*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOE,GPIO_Pin_14);                                              //使能NB
}

void NB_DeviceInit(void)
{
  NB_GPIOInit();
  NB_uart3_init();
}

void NB_HaveReceive(void)
{
  if(USART3_UartDataFinishFlag)
  {
    NB_RxFlag = 1;
    USART3_UartDataFinishFlag = 0;
    if(ExpectReceDataCEREG("\r\n+NSONMI:",&Rec_length) == 0)
    {
      NB_RxDataCount++;
      NB_RxFlag = 0;
      
      NB_CycReadTim = 0;                                                        //每次有新的数据 计数就清零
      
 //     ThereHaveSM = 0;                                                          //在有新消息的时候，旧消息将被覆盖
      
      if(NSORF_ComFlag == 1)                                                           //读取命令
      {
        if(NB_ContrastData("\r\nOK\r\n"))
        {
          NSORF_ComFlag = 2;
          NB_ReceOKFlag  = 1; 
          NB_RxFlag = 1;
        }
      }
      if(CSQFlag)
      {
        if(NB_ContrastData("\r\nOK\r\n"))
        {
          NB_ReceOKFlag  = 1; 
          NB_RxFlag = 1;
        }
      }
    }
    else
    {
      if(NSORF_ComFlag == 1)                                                           //读取命令
      {
        if(NB_ContrastData("\r\nOK\r\n"))
        {
          NSORF_ComFlag = 2;
          NB_ReceOKFlag  = 1; 
        }
      }
      else
      {
        if(NB_ContrastData("\r\nOK\r\n"))                                     //对比数据
        {
           NB_ReceOKFlag  = 1; 
           if(NSOST_ComFlag == 1)
             NSOST_ComFlag = 2;
        }
        if(ExpectReceDataCEREG("\r\n+CEREG:",&NB_NetSign) == 0)
        {
          if(NB_NetSign == 1)
          {
            NB_NetSign = 0;
            CEREG_Flag = 1;
          }
        }
          
      }
    }
  }
}

void CloseGPRS(void)
{
  if(!CloseGPRSFlag)
  {
    if(CloseGPRSTim >= 100)
    {
      CloseGPRSTim = 0;
      GPRS_Close();
    }
  }
  USART2_ReceiveData();
}

void NB_AutoConnect(void)
{
  switch(CMDSuccess)
  {
    case 0:                                             //AT
    {
      if(AT_Test())
        CMDSuccess++;
      break;
    }
    case 1:                                             //AT+CGSN       获取MAC地址
    {
      if(receive_Mac())
      {
        CMDSuccess++;
      }
      break;
    }
    case 2:                                             //AT+NCONFIG    配置为自动入网
    {
      if(AutoNet())
        CMDSuccess++;
      break;
    }   
    case 3:                                             //AT+NBAND      设置工作频段
    {
      if(SetBand())
        CMDSuccess++;
      break;
    }
    case 4:                                             //AT+CFUN       设置模块为全功能 1全功能 0 最小功能
    {
      if(SetFunc(1))
        CMDSuccess++;
      break;
    }
    case 5:                                             //AT+NRB        重启模块
    {
      if(ReBoot())
        CMDSuccess++;
      break;
    }
    case 6:                                             //AT+CEREG      检查网络信号状态
    {
      if(CheckNet())
        CMDSuccess++;
      break;
    }
    case 7:                                             //AT+NSOCR      创建SOCKET
    {
      if(NB_IOT_CreatSocket(UDPPort))
      {
        CMDSuccess = 0;
        NBInitFinishFlag = 1;
      }
      break;
    }
    default:
      break;
  }
}

/*初始化NB网络参数*/
void NB_Init(void)
{
  if(CloseGPRSFlag)
  {
    if(NBScramblingState)                                                          //扰码状态
    {
      switch(NBScramblingState)
      {
        case 1:                         //开启扰码 
        {
           SetScrambling(0); 
          break;
        }
        case 2:                         //闭扰码 
        {
           SetScrambling(1); 
          break;
        }
        default:
          break;
      }
    }
    else if(!NBInitFinishFlag)
    {
      if(NBTimCount >= 50)                                                         //250ms
      {
        NBTimCount = 0;
        NB_AutoConnect();                                                       //NB自动入网
      }  
    }
  }
}

/*发送数据*/
void NB_CollectionAndSendData(void)
{  
  SensorCycleCollect();                                                         //采集气压、温湿度
  
  if(NB_DataSendTim >= SensorCycle && CollectionFinishFlag && !NB_ReceiveFirst && CompTimeFlag)                             //采集数据发送                          //100ms
  {
    NB_DataSendTim = 0;  
    
    SensorDataWrite2Buf(NBIOT);                                                 //将采集到的数据通过参数方式发送出去
      
    PDAMemoryDataWrite2Buf(NBIOT);                                              //将PDA中存储的数据发送出去
  }
}

/*NB相关操作*/
void NB_DO(void)
{
  if(NBInitFinishFlag)                                                          //初始化完成之后
  { 
#ifdef SensorEquip
    if(NetJoinFlag == 2)                                                        //入网成功
    {
      NB_CollectionAndSendData();                                                            //数据采集和发送
    }
#endif
    NB_ReceiveData();                                                         //对NB接收到的数据处理与网络回复（与GPRS方式相同）
    PDAOperationUpload();                                                       //PDA操作上传（与GPRS方式相同）
    
    NB_CycleReadBuf();                                                          //周期去读接收BUF  防止NB接收BUF中的数据没有被读出来
  }
}

void NBIOTOption(void)
{
  if(NBIOT)  
  {
    USART3_ReceiveData();
    NB_HaveReceive();                                                           //NB接收到数据判断
    
    CloseGPRS();                                                                //关闭GPRS
    
    NB_Init();                                                                //NB模块初始化
    
    NB_DO();                                                                  //NB接收数据和传感器的数据采集与发送
  }
}