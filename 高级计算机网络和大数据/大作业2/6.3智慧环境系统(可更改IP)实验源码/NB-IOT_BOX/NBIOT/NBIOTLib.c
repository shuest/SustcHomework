#include "NBIOTLib.h"
#include "NBIOT.h"
#include "usart.h"
#include "gpio.h"

extern uint8_t NetJoinFlag;
extern uint8_t NBIOT;
extern void USART3_ReceiveData(void);   /*�ж��Ƕ�������ɺ���*/
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
extern int UDPPort;                             //UDP�˿ں�
extern void NB_CycleReadBuf(void);
extern void SensorCycleCollect(void);
extern void PDAMemoryDataWrite2Buf(uint8_t NetMode);
extern void SensorDataWrite2Buf(uint8_t NetMode);
extern uint8_t CollectionFinishFlag;
extern uint8_t NB_ReceiveFirst;
extern uint8_t CompTimeFlag;                       //��ʱ��ɱ�־λ

uint8_t NB_NetSign = 0;
uint8_t CMDSuccess = 0;

void NB_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  
  
  /* ����NB POWERΪ�������ģʽ BLUE*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOE,GPIO_Pin_14);                                              //ʹ��NB
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
      
      NB_CycReadTim = 0;                                                        //ÿ�����µ����� ����������
      
 //     ThereHaveSM = 0;                                                          //��������Ϣ��ʱ�򣬾���Ϣ��������
      
      if(NSORF_ComFlag == 1)                                                           //��ȡ����
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
      if(NSORF_ComFlag == 1)                                                           //��ȡ����
      {
        if(NB_ContrastData("\r\nOK\r\n"))
        {
          NSORF_ComFlag = 2;
          NB_ReceOKFlag  = 1; 
        }
      }
      else
      {
        if(NB_ContrastData("\r\nOK\r\n"))                                     //�Ա�����
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
    case 1:                                             //AT+CGSN       ��ȡMAC��ַ
    {
      if(receive_Mac())
      {
        CMDSuccess++;
      }
      break;
    }
    case 2:                                             //AT+NCONFIG    ����Ϊ�Զ�����
    {
      if(AutoNet())
        CMDSuccess++;
      break;
    }   
    case 3:                                             //AT+NBAND      ���ù���Ƶ��
    {
      if(SetBand())
        CMDSuccess++;
      break;
    }
    case 4:                                             //AT+CFUN       ����ģ��Ϊȫ���� 1ȫ���� 0 ��С����
    {
      if(SetFunc(1))
        CMDSuccess++;
      break;
    }
    case 5:                                             //AT+NRB        ����ģ��
    {
      if(ReBoot())
        CMDSuccess++;
      break;
    }
    case 6:                                             //AT+CEREG      ��������ź�״̬
    {
      if(CheckNet())
        CMDSuccess++;
      break;
    }
    case 7:                                             //AT+NSOCR      ����SOCKET
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

/*��ʼ��NB�������*/
void NB_Init(void)
{
  if(CloseGPRSFlag)
  {
    if(NBScramblingState)                                                          //����״̬
    {
      switch(NBScramblingState)
      {
        case 1:                         //�������� 
        {
           SetScrambling(0); 
          break;
        }
        case 2:                         //������ 
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
        NB_AutoConnect();                                                       //NB�Զ�����
      }  
    }
  }
}

/*��������*/
void NB_CollectionAndSendData(void)
{  
  SensorCycleCollect();                                                         //�ɼ���ѹ����ʪ��
  
  if(NB_DataSendTim >= SensorCycle && CollectionFinishFlag && !NB_ReceiveFirst && CompTimeFlag)                             //�ɼ����ݷ���                          //100ms
  {
    NB_DataSendTim = 0;  
    
    SensorDataWrite2Buf(NBIOT);                                                 //���ɼ���������ͨ��������ʽ���ͳ�ȥ
      
    PDAMemoryDataWrite2Buf(NBIOT);                                              //��PDA�д洢�����ݷ��ͳ�ȥ
  }
}

/*NB��ز���*/
void NB_DO(void)
{
  if(NBInitFinishFlag)                                                          //��ʼ�����֮��
  { 
#ifdef SensorEquip
    if(NetJoinFlag == 2)                                                        //�����ɹ�
    {
      NB_CollectionAndSendData();                                                            //���ݲɼ��ͷ���
    }
#endif
    NB_ReceiveData();                                                         //��NB���յ������ݴ���������ظ�����GPRS��ʽ��ͬ��
    PDAOperationUpload();                                                       //PDA�����ϴ�����GPRS��ʽ��ͬ��
    
    NB_CycleReadBuf();                                                          //����ȥ������BUF  ��ֹNB����BUF�е�����û�б�������
  }
}

void NBIOTOption(void)
{
  if(NBIOT)  
  {
    USART3_ReceiveData();
    NB_HaveReceive();                                                           //NB���յ������ж�
    
    CloseGPRS();                                                                //�ر�GPRS
    
    NB_Init();                                                                //NBģ���ʼ��
    
    NB_DO();                                                                  //NB�������ݺʹ����������ݲɼ��뷢��
  }
}