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
uint8_t NBTimCount = 0;                                 //NB����
uint8_t AbnormalTimCount = 0;                              //�쳣����
uint8_t ElecTimCount = 0;                               //������ʱ
uint16_t CollectionCount = 0;                               //��Ϣ�ɼ�����
uint16_t NB_DataSendTim = 0;                             //NB��������
uint16_t GPRS_DataSendTim = 0;                          //GPRS��������

uint16_t GPS_TimCount = 0;                                   //GPS��ʱ������ֵ
uint8_t GPS_UploadTimCount = 0;                                //�ϴ����ڼ���
uint8_t GPS_UartTim = 0;                                //���ڽ��� ����ʶ�𴮿����ݽ��ս���
uint8_t NetJoinTimCount = 0;                              //��¼����
uint8_t ACKTimCount = 0;                                //Ӧ��ʱ��
uint16_t HeartbeatTimCount = 0;                         //������Ƶ�ʼ���
uint8_t TernimalACKTimCount = 0;                         //�豸����������͵�ACK
uint8_t SendHeartBeatCount = 0;                         //�������������
uint8_t SwipingCardTimCount = 0;                           //ˢ������
uint8_t WarningTimCount = 0;                            //��������
uint16_t GPRSTimCount = 0;                               //GPRS����
uint8_t USART1_TimCount = 0;                                 //����1���ռ���
uint8_t USART2_TimCount = 0;                                 //����2���ռ���
uint8_t USART3_TimCount = 0;                                 //����3���ռ���
uint8_t USART4_TimCount = 0;                           //LORA(UART4)���ռ���
uint8_t LORATimCount = 0;                               //LORA��ʶ���ü���
uint8_t ScanReceiveTimCount = 0;                               //ɨ����յ�����
uint16_t LoraSensorCycleCount = 0;                      //LORA�������ϴ�����

uint8_t UpliftPoleTimCount = 0;                         //̧�˼���
uint8_t PDADataSendTimCount = 0;                        //PDA������ͨ��NB���͵ļ���

uint16_t GPRSTim = 0;                               //GPRS��ѯʱ��
uint8_t GPRSCheckConTim = 0;                            //GPRS��ѯ�ź�����
uint8_t GPRS_CheckCount = 0;                            //GPRS������
uint8_t WarningBeeTim = 0;                              //��������

uint8_t CloseLedORScreeTim = 0;                         //�صƻ�����������
uint8_t LockChangeTim = 0;                              //���䶯����
uint8_t SetGPSModeTim = 0;                              //����GPSģʽ����

uint8_t CloseGPRSTim = 0;                               //��GPRS����

uint8_t DisplayTim = 0;                                 //ˢ����ʾ����
uint8_t NB_CycReadTim = 0;                              //NB���ڶ�������      BUF����

uint8_t WarningBeeFlag = 0;
uint8_t BeginSendHeartBeat = 0;
uint8_t BegainCheckGPRS = 0;

uint8_t Tab_Local_ID[8] = {0};            //����ID
uint8_t IP_Address[15] = {"118.126.90.148"};                   //IP��ַ
uint8_t TCP_Port[5] = {"50005"};                      //TCP�˿ں�
uint8_t UDP_Port[5] = {"60005"};                      //UDP�ſں�
int UDPPort = 60005;
uint8_t PDA_Password_ID[10] = {0};        //����PDA����
uint16_t GPS_UploadCycle = 0;                  //GPS�ϴ�����ʱ��	
uint8_t GPS_WorkMode = 0;                     ///GPS����ģʽ
uint16_t Sensor_UploadCycle = 0;               //�������ϴ�����ʱ��
uint8_t Sensor_WorkMode = 0;                  //����������ģʽ
uint16_t HeartBeatCycleTime = 0;                //����������ʱ��
uint8_t LockState = 0;                          //���ĵ�ǰ״̬
uint8_t PoleState = 0;                          //�˵ĵ�ǰ״̬
uint32_t OperationNum = 0;                       //��������
uint8_t WarningTime[7] = {0};                   //����ʱ��
uint8_t WainingCount = 0;                       //��������
uint8_t WarningState = 0;                       //����״̬
uint8_t PDADataNum = 0;                         //��ʪ����Ϣ����
uint8_t PDADataNumFull = 0;                         //��ʪ����Ϣ������5��
uint8_t PDACloseLockNum = 0;                    //PDA������δ����ʱ�Ĺ�����
uint8_t PDACloseLockNumFull = 0;
uint8_t PDAOpenLockNum = 0;                     //PDA������δ����ʱ�Ŀ�����
uint8_t PDAOpenLockNumFull = 0;
uint8_t NBScramblingState = 0;                  //NB����״̬

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
extern uint8_t GPS_WorkMode;                     ///GPS����ģʽ
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



    /*����GPIO*/	
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//   /*GPRS��NBѡ�񿪹�*/
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


    /*������*/	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOC, &GPIO_InitStructure);
            
    I2C_Init1();
}

//���r�����ú���
void TIM2_Configuration(uint16_t period,uint16_t prescaler)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//������r���Д�Դ
  
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
  
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//���_tim2���Д�
  TIM_Cmd(TIM2,ENABLE);
}

void TIM3_Configuration(uint16_t period,uint16_t prescaler)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//������r���Д�Դ
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
  
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//���_tim3���Д�
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
* �� �� ��;ReadParaFromEEprom
* ��������:��EEPROM�ж�ȡ����Ҫ�����ò���
* ��    ��: 
* ��    ��: 
* ע���ú����а��������е���Ҫ�����������޸ġ�
*******************************************************************************/
void ReadParaFromEEprom(void)
{
  uint8_t m = 0;
  uint8_t dx = 0;
  uint8_t EEPROM_Data_Test[255] = {0};     //EEPROM ����
  uint16_t Self_Test = 0;;               //�ϵ��Լ��־
  
  AT24CXX_Read(0x00,EEPROM_Data_Test,255);          //�ٴζ�ȡ����
         
  Self_Test=(EEPROM_Data_Test[1]<<8)+EEPROM_Data_Test[2];
  
  Watchdog();
  
  if(Self_Test== 0x5555)    
  {
    memcpy(Tab_Local_ID,&EEPROM_Data_Test[ID_Address], 8);     //��������ID 
//    memcpy(IP_Address\,&EEPROM_Data_Test[Ip_Address], 15);     //����IP                                          �޸�IP�Ͷ˿ڣ�����IP_Address\TCP_Port\UDP_Port����
//    memcpy(TCP_Port,&EEPROM_Data_Test[TCP_Address], 5);     //����TCP�˿ں�
//    memcpy(UDP_Port,&EEPROM_Data_Test[UDP_Address], 5);     //����UDP�˿ں�
    UDPPort = NMEA_Str2num(UDP_Port,&dx);
    
    memcpy(PDA_Password_ID,&EEPROM_Data_Test[PsWord_Address], 10);     //������������
    
    GPS_UploadCycle = EEPROM_Data_Test[GPSCycle_Address] + (EEPROM_Data_Test[GPSCycle_Address+1] << 8);//����GPS�ϴ�����
    memcpy(&GPS_WorkMode,&EEPROM_Data_Test[GPSMode_Address], 1);     //����GPS����ģʽ   
    
    Sensor_UploadCycle = EEPROM_Data_Test[SensorCycle_Address] + (EEPROM_Data_Test[SensorCycle_Address+1] << 8);//�����������ϴ�����  
    memcpy(&Sensor_WorkMode,&EEPROM_Data_Test[SensorMode_Address], 1);     //��������������ģʽ
    
    HeartBeatCycleTime = EEPROM_Data_Test[HeartBeat_Address] + (EEPROM_Data_Test[HeartBeat_Address+1] << 8);//�����������ϴ�����    
    
    memcpy(&LockState,&EEPROM_Data_Test[LockState_Address], 1);     //�������ĵ�ǰ״̬
    
    memcpy(&PoleState,&EEPROM_Data_Test[PoleState_Address], 1);     //�����˵ĵ�ǰ״̬
    
    OperationNum = EEPROM_Data_Test[Operation_Address] + (EEPROM_Data_Test[Operation_Address+1]*256) + (EEPROM_Data_Test[Operation_Address+2]*256*256) + (EEPROM_Data_Test[Operation_Address+3]*256*256*256);//������������

    WarningState = EEPROM_Data_Test[WarningState_Address];            //��ȡ������״̬
    
    WainingCount = EEPROM_Data_Test[WarningCount_Address];            //��ȡ����������
    
    memcpy(WarningTime,&EEPROM_Data_Test[WarningTime_Address], 7);    //��ȡ������ʱ��
    
    AT24CXX_Read(PDADataNum_Address,&PDADataNum,1);                    //��ȡ��ʪ����Ϣ����
    
    UART4_Put(&EEPROM_Data_Test[Lora_Address],23);   //��EEPROM�ж�ȡ������������ LORA
    
    LORATimCount = 0;
    for(m=0;m<10;)    //�ȴ�1S�鿴�����Ƿ�ɹ�
    {
        if(LORATimCount >= 1 && USART4_UartReceiving)                                     //10ms
        {
          LORATimCount = 0;
          if(USART4_ANum == USART4_RxCount)                                                 //���ݽ������
          {
            USART4_UartDataFinishFlag = 1;
            USART4_UartReceiving = 0;                                                    //���ڽ������ݽ���
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
        if(LORATimCount >= 5) // 1s ʱ�䵽
        {
            LORATimCount=0;
            
            ClearScreen23();
            LCD12864_wstr(2,0,"LORA����ʧ��");
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
    LCD12864_wstr(2,0,"�豸�洢����");
    LCD12864_wstr(3,0,"����������");
  }
}


void NB_BoxInit(void)
{
  SystemInit();
  delay_init(72); 
  
  WatchDogInit();                                                               //���Ź���ʼ��
  
  GPIO_init();                                                                  //���ð�����LED   
  if(LOCK_CHECK)
    NBIOT = 1;
  else
    GPRS = 1;
    
  
  TIM3_Configuration(1000,360);                                               //��ʱ5����
  TIM2_Configuration(4000,3600);                                               //��ʱ200����
  NVIC_TIM_Configuration();
  
  Adc_Init();
#ifdef SensorEquip
  SHT10_IOConfig();                                                             //��ʼ����ʪ��
  bmp280Init();                                                                //��ʼ��������ѹ
#elif LockEquip
  LOCK_GPIOConf();                                                              //���ĳ�ʼ��
#elif ICCard
  mfrc522_GPIO_ini();    //mfrc522  GPIO��ʼ��
  InitializeSystem();	 //mfrc522 ���߳�ʼ��
#endif
  
  AT24CXX_Init();                                                               //E2PROM��ʼ�� 
  
  LCD12864_Init();                                                              //��ʼ��LCD��
  delay_ms(10);
  LCD1264_clear();
  delay_ms(10);         
  
  LCD12864_wstr(2,0,"    ��̫�Ƽ�  ");
#ifdef LockEquip
  LCD12864_wstr(3,0,"  LOCK SYSTEM ");
#elif SensorEquip
  LCD12864_wstr(3,0,"   E M SYSTEM ");
#elif ICCard
  LCD12864_wstr(3,0,"   P.K SYSTEM ");
#endif
  
  /*��ش��ڳ�ʼ��*/
  GPS_Init();                                                                   //����5 9600
  
  if(NBIOT)
  {
    NB_DeviceInit();                                                            //����3���� 9600  
  }
  
  Initial_UART1(57600);                                                         //��ʼ������1  57600
  
  LORA_DeviceInit();                                                            //LORA�豸�ĳ�ʼ��  9600
  
  GPRS_DeviceInit();                                                            //GPRS��ʼ��  57600
  if(GPRS)
  {
    GPIO_SetBits(GPIOE,GPIO_Pin_14);                                              //��NB
  }
  /*��ش��ڳ�ʼ��*/
  NVIC_Configuration();
  
  delay_s(2);
  Parameter_Check();                                                             //�ϵ���Բ����Ƿ����ã�����LORA,��ȡLORA SN����5λ�Զ�����8λ ǰ3λ��0
  
  ReadParaFromEEprom();                                                         //��EEPROM�ж�ȡ��������Ҫ�������ڸú�����
  
  LCD1264_clear();
  delay_ms(10);     
  
  LCD12864_wstr(4,2," OFF");
  LCD12864_wstr(4,6," OFF");
}

void CycleCheckGPRS(void)
{
  if((GPRSCheckConTim > 100) && (NetJoinFlag == 2))       //20s ��ǰ��GPRS���ͺͽ���
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
  if((HeartbeatTimCount >= HeartBeatCycleTime*5) && (NetJoinFlag == 2))                               //20Sһ�ε�������
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
  if(AbnormalTimCount >= 30)                                                  //300msɨ��һ���쳣
  {
    AbnormalTimCount = 0;
    if(NeedRBTFlag)                                                             //��Ҫ����
    {
      if(NBIOT)
      {
        NeedRBTFlag = 0;
        CloseGPRSFlag = 0;
        NBInitFinishFlag = 0;
        CMDSuccess = 0;
        NetJoinFlag = 0;
        LCD12864_wstr(4,2," OFF");
        GPIO_SetBits(GPIOE,GPIO_Pin_14);                                              //ʧ��NB
        delay_s(2);
        GPIO_ResetBits(GPIOE,GPIO_Pin_14);                                              //ʹ��NB
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
  if(ElecTimCount >= 100)                                                      //500msһ�εĵ������
  {
    ElecTimCount = 0;
    if(DAT_Vol() < 3.5)
    {
      WarningFlag = 1;
      WarningBeeFlag = 1;
    }
    else                                                                      //�������
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
  if(WarningBeeTim >= 5 && WarningBeeFlag)                                    //����time 1s
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
    CycleCheckGPRS();                                                           //GPS�������ڼ��
  }
  
  CycleSendHeartBeat();                                                         //���������ڷ���
  
  CycleAbnormalCheck();                                                         ////�쳣���ڼ��
  
  CycleCheckElec();                                                             //�������ڼ��
  
  CycleCheckWarning();                                                          //�������ڼ��
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
    LCD12864_wstr(1,1,IDArray);                                              //�豸ID
    LCD12864_wstr(1,6,elec);                                              //��ǰ��ѹ
    if(NBIOT)                                                                     //����ģʽ
      LCD12864_wstr(4,0," NB ");
    else if(GPRS)
      LCD12864_wstr(4,0,"GPRS");
    
    switch(GPS_WorkMode)                                      //��λģʽ
    {
      case 0x01:                                              //GPS
      {
        LCD12864_wstr(4,5,"GP");
        break;
      }
      case 0x02:                                              //����
      {
        LCD12864_wstr(4,5,"BD");
        break;
      }
      case 0x03:                                              //���
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

