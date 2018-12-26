#include "gpio.h"
#include "delay.h"
#include "NBIOTLib.h"
#include "GPSLib.h"
#include "lock.h"
#include "usart.h"
#include "SHT10.h"
#include "24cxx.h" 
#include "gprs.h"
#include "GPRSLib.h"
#include "BMP280.h"
#include "UM402.h"
#include "UM402Lib.h"
#include "LCD12864.h"
#include "PCF8563.h" 
#include "adc.h"
#include "myiic.h"
#include "string.h"
#include "UART4.h"

int Tim2Count = 0;
uint8_t NBTimCount = 0;                                 //NB计数
uint8_t AbnormalTimCount = 0;                              //异常计数
uint8_t ElecTimCount = 0;                               //电量定时
uint16_t CollectionCount = 0;                               //信息采集周期
uint16_t NB_DataSendTim = 0;                             //NB发送周期
uint16_t GPRS_DataSendTim = 0;                          //GPRS发送周期

uint16_t GPS_TimCount = 0;                                   //GPS定时器计数值
uint8_t GPS_UploadTimCount = 0;                                //上传周期计数
uint8_t GPS_UartTim = 0;                                //串口接受 用来识别串口数据接收结束
uint8_t NetJoinTimCount = 0;                              //登录周期
uint8_t ACKTimCount = 0;                                //应答时间
uint16_t HeartbeatTimCount = 0;                         //心跳包频率计数
uint8_t TernimalACKTimCount = 0;                         //设备向服务器发送的ACK
uint8_t SendHeartBeatCount = 0;                         //心跳包检测周期
uint8_t SwipingCardTimCount = 0;                           //刷卡计数
uint8_t WarningTimCount = 0;                            //报警计数
uint16_t GPRSTimCount = 0;                               //GPRS计数
uint8_t USART1_TimCount = 0;                                 //串口1接收计数
uint8_t USART2_TimCount = 0;                                 //串口2接收计数
uint8_t USART3_TimCount = 0;                                 //串口3接收计数
uint8_t USART4_TimCount = 0;                           //LORA(UART4)接收计数
uint8_t LORATimCount = 0;                               //LORA初识配置计数
uint8_t ScanReceiveTimCount = 0;                               //扫描接收得数据
uint16_t LoraSensorCycleCount = 0;                      //LORA传感器上传周期

uint8_t UpliftPoleTimCount = 0;                         //抬杆计数
uint8_t PDADataSendTimCount = 0;                        //PDA的数据通过NB发送的计数

uint16_t GPRSTim = 0;                               //GPRS查询时间
uint8_t GPRSCheckConTim = 0;                            //GPRS查询信号周期
uint8_t GPRS_CheckCount = 0;                            //GPRS检查计数
uint8_t WarningBeeTim = 0;                              //报警周期

uint8_t CloseLedORScreeTim = 0;                         //关灯或者清屏计数
uint8_t LockChangeTim = 0;                              //锁变动计数
uint8_t SetGPSModeTim = 0;                              //设置GPS模式周期

uint8_t CloseGPRSTim = 0;                               //关GPRS周期

uint8_t DisplayTim = 0;                                 //刷新显示周期
uint8_t NB_CycReadTim = 0;                              //NB周期读接收区      BUF周期

uint8_t WarningBeeFlag = 0;
uint8_t BeginSendHeartBeat = 0;
uint8_t BegainCheckGPRS = 0;

uint8_t Tab_Local_ID[8] = {0};            //本机ID
uint8_t IP_Address[15] = {"118.126.90.148"};                   //IP地址
uint8_t TCP_Port[5] = {"50005"};                      //TCP端口号
uint8_t UDP_Port[5] = {"60005"};                      //UDP团口号
int UDPPort = 60005;
uint8_t PDA_Password_ID[10] = {0};        //本机PDA密码
uint16_t GPS_UploadCycle = 0;                  //GPS上传周期时间	
uint8_t GPS_WorkMode = 0;                     ///GPS工作模式
uint16_t Sensor_UploadCycle = 0;               //传感器上传周期时间
uint8_t Sensor_WorkMode = 0;                  //传感器工作模式
uint16_t HeartBeatCycleTime = 0;                //心跳包周期时间
uint8_t LockState = 0;                          //锁的当前状态
uint8_t PoleState = 0;                          //杆的当前状态
uint32_t OperationNum = 0;                       //操作次数
uint8_t WarningTime[7] = {0};                   //报警时间
uint8_t WainingCount = 0;                       //报警次数
uint8_t WarningState = 0;                       //报警状态
uint8_t PDADataNum = 0;                         //温湿度信息数量
uint8_t PDADataNumFull = 0;                         //温湿度信息数量满5条
uint8_t PDACloseLockNum = 0;                    //PDA在网络未连接时的关锁数
uint8_t PDACloseLockNumFull = 0;
uint8_t PDAOpenLockNum = 0;                     //PDA在网络未连接时的开锁数
uint8_t PDAOpenLockNumFull = 0;
uint8_t NBScramblingState = 0;                  //NB扰码状态

extern uint8_t CloseGPRSFlag;
extern void GPRS_Start(void);
extern u8 GPRSCMDNum;
extern u8 GPRSInitFinishFlag;
extern void GPRS_OFF(void);
extern void GPRS_GpioInit(void);
extern void NB_GPIOInit(void);
extern uint8_t CMDSuccess;
extern uint8_t NBInitFinishFlag;
extern unsigned char Tab_Local_ID[8];
extern uint8_t GPS_WorkMode;                     ///GPS工作模式
extern uint8_t GPS_WorkMode;
extern void mfrc522_GPIO_ini(void);
extern void InitializeSystem(void);
extern uint8_t ConnectSurvival;
extern uint8_t USART2_UartReceiving;
extern uint8_t AbnormalTimCount;
extern uint8_t NeedRBTFlag;
extern uint8_t ElecTimCount;
extern uint16_t HeartbeatTimCount;
extern uint8_t SendHeartBeatCount;
extern uint16_t HeartBeatCycleTime;
extern uint8_t NetJoinFlag;
extern uint8_t WarningFlag;
extern uint8_t GPRSCheckConTim;
extern uint8_t GPRS_CheckCount;
extern uint8_t WarningBeeTim;
extern u8 GPRSSending;
extern uint8_t BegainCheckGPRS;

void Watchdog(void)
{
  RESET_WDI_H();
  delay_us(20);
  RESET_WDI_L();
  delay_us(20);
}

void WatchDogInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOC,GPIO_Pin_1);
}

void blink_BLUE(void)
{
   if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))
     GPIO_ResetBits(GPIOA,GPIO_Pin_6);
   else
      GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

void blink_RED(void)
{
   if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7))
     GPIO_ResetBits(GPIOA,GPIO_Pin_7);
   else
      GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);            

    /*12864 GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOE, &GPIO_InitStructure);

//    /*bmp085*/	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; //SDA 12   SCL 13      
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   
//    GPIO_Init(GPIOB, &GPIO_InitStructure);



    /*按键GPIO*/	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//   AUTO
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//   MANUAL
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//   LOCK
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//  UNLOCK
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//   /*GPRS和NB选择开关*/
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//  REMOVE_ALARM
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//  LIFT_ROD
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /*LED*/	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    /*蜂鸣器*/	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOC, &GPIO_InitStructure);
            
    I2C_Init1();
}

//定時器配置函數
void TIM2_Configuration(uint16_t period,uint16_t prescaler)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清楚定時器中斷源
  
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
  
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//打開tim2的中斷
  TIM_Cmd(TIM2,ENABLE);
}

void TIM3_Configuration(uint16_t period,uint16_t prescaler)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清楚定時器中斷源
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
  
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//打開tim3的中斷
  TIM_Cmd(TIM3,ENABLE);
}

void NVIC_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
}

/*******************************************************************************
* 函 数 名;ReadParaFromEEprom
* 函数功能:从EEPROM中读取出重要的配置参数
* 输    入: 
* 输    出: 
* 注：该函数中包含了所有的重要参数，谨慎修改。
*******************************************************************************/
void ReadParaFromEEprom(void)
{
  uint8_t m = 0;
  uint8_t dx = 0;
  uint8_t EEPROM_Data_Test[255] = {0};     //EEPROM 数据
  uint16_t Self_Test = 0;;               //上电自检标志
  
  AT24CXX_Read(0x00,EEPROM_Data_Test,255);          //再次读取数据
         
  Self_Test=(EEPROM_Data_Test[1]<<8)+EEPROM_Data_Test[2];
  
  Watchdog();
  
  if(Self_Test== 0x5555)    
  {
    memcpy(Tab_Local_ID,&EEPROM_Data_Test[ID_Address], 8);     //读出本机ID 
//    memcpy(IP_Address\,&EEPROM_Data_Test[Ip_Address], 15);     //读出IP                                          修改IP和端口，更改IP_Address\TCP_Port\UDP_Port参数
//    memcpy(TCP_Port,&EEPROM_Data_Test[TCP_Address], 5);     //读出TCP端口号
//    memcpy(UDP_Port,&EEPROM_Data_Test[UDP_Address], 5);     //读出UDP端口号
    UDPPort = NMEA_Str2num(UDP_Port,&dx);
    
    memcpy(PDA_Password_ID,&EEPROM_Data_Test[PsWord_Address], 10);     //读出口令密码
    
    GPS_UploadCycle = EEPROM_Data_Test[GPSCycle_Address] + (EEPROM_Data_Test[GPSCycle_Address+1] << 8);//读出GPS上传周期
    memcpy(&GPS_WorkMode,&EEPROM_Data_Test[GPSMode_Address], 1);     //读出GPS工作模式   
    
    Sensor_UploadCycle = EEPROM_Data_Test[SensorCycle_Address] + (EEPROM_Data_Test[SensorCycle_Address+1] << 8);//读出传感器上传周期  
    memcpy(&Sensor_WorkMode,&EEPROM_Data_Test[SensorMode_Address], 1);     //读出传感器工作模式
    
    HeartBeatCycleTime = EEPROM_Data_Test[HeartBeat_Address] + (EEPROM_Data_Test[HeartBeat_Address+1] << 8);//读出心跳包上传周期    
    
    memcpy(&LockState,&EEPROM_Data_Test[LockState_Address], 1);     //读出锁的当前状态
    
    memcpy(&PoleState,&EEPROM_Data_Test[PoleState_Address], 1);     //读出杆的当前状态
    
    OperationNum = EEPROM_Data_Test[Operation_Address] + (EEPROM_Data_Test[Operation_Address+1]*256) + (EEPROM_Data_Test[Operation_Address+2]*256*256) + (EEPROM_Data_Test[Operation_Address+3]*256*256*256);//读出操作次数

    WarningState = EEPROM_Data_Test[WarningState_Address];            //读取出报警状态
    
    WainingCount = EEPROM_Data_Test[WarningCount_Address];            //读取出报警次数
    
    memcpy(WarningTime,&EEPROM_Data_Test[WarningTime_Address], 7);    //读取出报警时间
    
    AT24CXX_Read(PDADataNum_Address,&PDADataNum,1);                    //读取温湿度信息数量
    
    UART4_Put(&EEPROM_Data_Test[Lora_Address],23);   //从EEPROM中读取到的配置设置 LORA
    
    LORATimCount = 0;
    for(m=0;m<10;)    //等待1S查看配置是否成功
    {
        if(LORATimCount >= 1 && USART4_UartReceiving)                                     //10ms
        {
          LORATimCount = 0;
          if(USART4_ANum == USART4_RxCount)                                                 //数据接收完成
          {
            USART4_UartDataFinishFlag = 1;
            USART4_UartReceiving = 0;                                                    //串口接受数据结束
            USART4_RxCount = 0;
          }
          else
          {
            USART4_ANum = USART4_RxCount;
          }
          
        }
        if(USART4_UartDataFinishFlag)
        {
          USART4_UartDataFinishFlag = 0;
          if(USART4_RxBuf[0]==0x24)
          {
              if((USART4_RxBuf[1]==EEPROM_Data_Test[185])&(USART4_RxBuf[2]==EEPROM_Data_Test[186]))
              {
                  UM402_Test_Flage=0xA0;
                  USART4_RxCount=0x00;
                  break;
              }
          }
        }
        if(LORATimCount >= 5) // 1s 时间到
        {
            LORATimCount=0;
            
            ClearScreen23();
            LCD12864_wstr(2,0,"LORA配置失败");
            UM402_Test_Flage=0x50;
            USART4_RxCount=0x00;
            break;
        }
    }
    LORA_SETA_L();
    LORA_SETB_L();
  }
  else
  {
    ClearScreen23();
    LCD12864_wstr(2,0,"设备存储错误，");
    LCD12864_wstr(3,0,"请重新启动");
  }
}


void NB_BoxInit(void)
{
  SystemInit();
  delay_init(72); 
  
  WatchDogInit();                                                               //看门狗初始化
  
  GPIO_init();                                                                  //配置按键和LED   
  if(LOCK_CHECK)
    NBIOT = 1;
  else
    GPRS = 1;
    
  
  TIM3_Configuration(1000,360);                                               //定时5毫秒
  TIM2_Configuration(4000,3600);                                               //定时200毫秒
  NVIC_TIM_Configuration();
  
  Adc_Init();
#ifdef SensorEquip
  SHT10_IOConfig();                                                             //初始化温湿度
  bmp280Init();                                                                //初始化数字气压
#elif LockEquip
  LOCK_GPIOConf();                                                              //锁的初始化
#elif ICCard
  mfrc522_GPIO_ini();    //mfrc522  GPIO初始化
  InitializeSystem();	 //mfrc522 天线初始化
#endif
  
  AT24CXX_Init();                                                               //E2PROM初始化 
  
  LCD12864_Init();                                                              //初始化LCD屏
  delay_ms(10);
  LCD1264_clear();
  delay_ms(10);         
  
  LCD12864_wstr(2,0,"    泛太科技  ");
#ifdef LockEquip
  LCD12864_wstr(3,0,"  LOCK SYSTEM ");
#elif SensorEquip
  LCD12864_wstr(3,0,"   E M SYSTEM ");
#elif ICCard
  LCD12864_wstr(3,0,"   P.K SYSTEM ");
#endif
  
  /*相关串口初始化*/
  GPS_Init();                                                                   //串口5 9600
  
  if(NBIOT)
  {
    NB_DeviceInit();                                                            //串口3配置 9600  
  }
  
  Initial_UART1(57600);                                                         //初始化串口1  57600
  
  LORA_DeviceInit();                                                            //LORA设备的初始化  9600
  
  GPRS_DeviceInit();                                                            //GPRS初始化  57600
  if(GPRS)
  {
    GPIO_SetBits(GPIOE,GPIO_Pin_14);                                              //关NB
  }
  /*相关串口初始化*/
  NVIC_Configuration();
  
  delay_s(2);
  Parameter_Check();                                                             //上电测试参数是否配置，配置LORA,读取LORA SN号码5位自动补齐8位 前3位补0
  
  ReadParaFromEEprom();                                                         //从EEPROM中读取参数，重要参数都在该函数中
  
  LCD1264_clear();
  delay_ms(10);     
  
  LCD12864_wstr(4,2," OFF");
  LCD12864_wstr(4,6," OFF");
}

void CycleCheckGPRS(void)
{
  if((GPRSCheckConTim > 100) && (NetJoinFlag == 2))       //20s 当前无GPRS发送和接收
  {
    GPRSCheckConTim = 0;
    BegainCheckGPRS = 1;
  }
  if(GPRS_CheckCount >= 40 && BegainCheckGPRS && (NetJoinFlag == 2) && !USART2_UartReceiving && !GPRSSending)                                //200ms
  {
    GPRS_CheckCount = 0;
    CheckGPRSConnect(); 
  }
}

void CycleSendHeartBeat(void)
{
  if((HeartbeatTimCount >= HeartBeatCycleTime*5) && (NetJoinFlag == 2))                               //20S一次的心跳包
  {
    HeartbeatTimCount = 0;
    BeginSendHeartBeat = 1;
  }
  if(SendHeartBeatCount >= 40 && BeginSendHeartBeat && (NetJoinFlag == 2) && !USART2_UartReceiving && !BegainCheckGPRS)                           //200ms
  {
    SendHeartBeatCount = 0;
    if(NBIOT)
      SendHeartBeatPackage();
    else if(GPRS && ConnectSurvival)
      M26_HeartBeat();
  }
}

void CycleAbnormalCheck(void)
{
  if(AbnormalTimCount >= 30)                                                  //300ms扫描一次异常
  {
    AbnormalTimCount = 0;
    if(NeedRBTFlag)                                                             //需要重启
    {
      if(NBIOT)
      {
        NeedRBTFlag = 0;
        CloseGPRSFlag = 0;
        NBInitFinishFlag = 0;
        CMDSuccess = 0;
        NetJoinFlag = 0;
        LCD12864_wstr(4,2," OFF");
        GPIO_SetBits(GPIOE,GPIO_Pin_14);                                              //失能NB
        delay_s(2);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);                                              //使能NB
      }
      else if(GPRS)
      {
         NeedRBTFlag = 0;
         GPRSInitFinishFlag = 0;
         GPRSCMDNum = 0;
         NetJoinFlag = 0;
         LCD12864_wstr(4,2," OFF");
         GPRS_Start();
      }
    }
  }
}

void CycleCheckElec(void)
{
  if(ElecTimCount >= 100)                                                      //500ms一次的电量检测
  {
    ElecTimCount = 0;
    if(DAT_Vol() < 3.5)
    {
      WarningFlag = 1;
      WarningBeeFlag = 1;
    }
    else                                                                      //报警解除
    {
      WarningBeeFlag = 0;
      WarningFlag = 0;
      BEEP_L();
      LED_RED_L();
    }
  }
}

void CycleCheckWarning(void)
{
  if(WarningBeeTim >= 5 && WarningBeeFlag)                                    //报警time 1s
  {
    if(Read_LED_RED())
      LED_RED_L();
    else 
      LED_RED_H();
    
    if(Read_BEEP())
      BEEP_L();
    else
      BEEP_H();
  }
}

void CycleCheck(void)
{
  if(GPRS)
  {
    CycleCheckGPRS();                                                           //GPS连接周期检查
  }
  
  CycleSendHeartBeat();                                                         //心跳包周期发送
  
  CycleAbnormalCheck();                                                         ////异常周期检查
  
  CycleCheckElec();                                                             //电量周期检查
  
  CycleCheckWarning();                                                          //报警周期检查
}

void ElecChange(float Fele,uint8_t *ele)
{
  uint8_t PO = 0,One = 0;
  uint16_t elec10 = 0;
  elec10 = (uint16_t)(Fele*10);
  PO = elec10%10;
  One = elec10/10;
  
  *ele = One+0x30;
  ele++;
  *ele = '.';
  ele++;
  *ele = PO+0x30;
  ele++;
  *ele = 'V';
}

void IDChange(uint8_t *data,uint8_t *Array)
{
  signed char Buf[8] = {0};
  uint8_t i = 0;
  for(i = 0;i < 4;i++)
  {
    Buf[2*i] = (*data) >> 4;
    Buf[2*i+1] = (*data)&0x0f;
    data++;
  }
  for(i = 0;i < 8;i++)
  {
    if(Buf[i] >= 0 && Buf[i] <= 9)
      Buf[i] += '0';
    else        
      Buf[i] += 0x37;
    *Array = Buf[i];
    Array++;
  }
}

void DisPlay(void)
{
  uint8_t elec[5] = {0};
  uint8_t IDArray[9] = {0};
  float FElec = 0;
  uint8_t Test[10] = {0};
  if(DisplayTim >= 10)
  {
    DisplayTim = 0;
    RTC_OP(Test,GET_RTC_TIME);
    FElec = DAT_Vol();
    ElecChange(FElec,elec);
    LCD12864_wstr(1,6,elec);
    LCD12864_wstr(1,0,"ID");
    IDChange(&Tab_Local_ID[4],IDArray);
    LCD12864_wstr(1,1,IDArray);                                              //设备ID
    LCD12864_wstr(1,6,elec);                                              //当前电压
    if(NBIOT)                                                                     //连接模式
      LCD12864_wstr(4,0," NB ");
    else if(GPRS)
      LCD12864_wstr(4,0,"GPRS");
    
    switch(GPS_WorkMode)                                      //定位模式
    {
      case 0x01:                                              //GPS
      {
        LCD12864_wstr(4,5,"GP");
        break;
      }
      case 0x02:                                              //北斗
      {
        LCD12864_wstr(4,5,"BD");
        break;
      }
      case 0x03:                                              //混合
      {
        LCD12864_wstr(4,5,"GB");
        break;
      }
      default:
        break;
    }
  }
}

void TIM2_IRQHandler(void)                                                      //200ms
{
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  Tim2Count++;
  if(Tim2Count >= 5000)
      Tim2Count = 0;
  
  CollectionCount++;
  if(CollectionCount >= 65530)
    CollectionCount = 0; 
  
  GPS_TimCount++;
  if(GPS_TimCount >= 255)
    GPS_TimCount = 0;
  
  HeartbeatTimCount++;
  if(HeartbeatTimCount >= 65530)
    HeartbeatTimCount = 0;
  
  LORATimCount++;
  if(LORATimCount >= 255)
    LORATimCount = 0;
  
  ACKTimCount++;
  if(ACKTimCount >= 255)
    ACKTimCount = 0;
  
  LoraSensorCycleCount++;
  if(LoraSensorCycleCount >= 255)
    LoraSensorCycleCount = 0;
  
  UpliftPoleTimCount++;
  if(UpliftPoleTimCount >= 255)
    UpliftPoleTimCount = 0;
  
  GPRSTim++;
  if(GPRSTim >= 2000)
    GPRSTim = 0;
  
  GPRSCheckConTim++;
  if(GPRSCheckConTim >= 255)
    GPRSCheckConTim = 0;
  
  WarningBeeTim++;
  if(WarningBeeTim >= 255)
    WarningBeeTim = 0;
  
  CloseLedORScreeTim++;
  if(CloseLedORScreeTim >= 255)
    CloseLedORScreeTim = 0;
  
  DisplayTim++;
  if(DisplayTim >= 255)
    DisplayTim = 0;
  
  NB_CycReadTim++;
  if(NB_CycReadTim >= 255)
    NB_CycReadTim = 0;
}

void TIM3_IRQHandler(void)                                                      //5ms
{
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
  
  NBTimCount++;
  if(NBTimCount >= 200)
      NBTimCount = 0;
  
  AbnormalTimCount++;
  if(AbnormalTimCount >= 200)
    AbnormalTimCount = 0;
   
  ElecTimCount++;
  if(ElecTimCount >= 200)
    ElecTimCount = 0;
  
  NetJoinTimCount++;
  if(NetJoinTimCount >= 255)
   NetJoinTimCount = 0;
  
  NB_DataSendTim++;
  if(NB_DataSendTim >= 200)
    NB_DataSendTim = 0; 
  
  GPRS_DataSendTim++;
  if(GPRS_DataSendTim >= 200)
    GPRS_DataSendTim = 0; 
  
  GPS_UartTim++;
  if(GPS_UartTim > 200)
    GPS_UartTim = 0;
  
  GPS_UploadTimCount++;
  if(GPS_UploadTimCount >= 255)
    GPS_UploadTimCount = 0;
  
  TernimalACKTimCount++;
  if(TernimalACKTimCount >= 255)
    TernimalACKTimCount = 0;
  
  SendHeartBeatCount++;
  if(SendHeartBeatCount >= 255)
    SendHeartBeatCount = 0;
    
  SwipingCardTimCount++;
  if(SwipingCardTimCount >= 255)
    SwipingCardTimCount = 0;
  
  WarningTimCount++;
  if(WarningTimCount >= 255)
    WarningTimCount = 0;
  
  GPRSTimCount++;
  if(GPRSTimCount >= 255)
    GPRSTimCount = 0;
  
  USART1_TimCount++;
  if(USART1_TimCount >= 255)
    USART1_TimCount = 0;
  
  USART4_TimCount++;
  if(USART4_TimCount >= 255)
    USART4_TimCount = 0;
  
  ScanReceiveTimCount++;
  if(ScanReceiveTimCount >= 255)
    ScanReceiveTimCount = 0;
  
  PDADataSendTimCount++;
  if(PDADataSendTimCount >= 255)
    PDADataSendTimCount = 0;
  
  USART2_TimCount++;
  if(USART2_TimCount >= 255)
    USART2_TimCount = 0;
  
  GPRS_CheckCount++;
  if(GPRS_CheckCount >= 255)
    GPRS_CheckCount = 0;
  
  SetGPSModeTim++;
  if(SetGPSModeTim >= 255)
    SetGPSModeTim = 0;
  
  LockChangeTim++;
  if(LockChangeTim>= 255)
    LockChangeTim = 0;
  
  CloseGPRSTim++;
  if(CloseGPRSTim >= 255)
    CloseGPRSTim = 0;
  
  USART3_TimCount++;
  if(USART3_TimCount >= 255)
    USART3_TimCount = 0;
}

