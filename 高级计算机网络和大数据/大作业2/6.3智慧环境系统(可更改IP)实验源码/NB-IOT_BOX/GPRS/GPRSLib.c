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
extern uint16_t Sensor_UploadCycle;             //�������ϴ�����
//��ѹ
extern float BMP280_Pressure;
//����
extern float BMP280_High;
//�¶�
extern float BMP280_Tem;
extern uint8_t CollectionFinishFlag;
extern uint16_t GPRS_DataSendTim;
extern uint16_t SensorCycle;
extern uint8_t USART2_UartReceiving;
extern uint8_t CompTimeFlag;                       //��ʱ��ɱ�־λ
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
extern uint8_t ContrastData(uint8_t *string);                                     //�Ա�����
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

  /* ���ô���2 Tx��PA2��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 Rx��PA3��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  /* ����2��ʼ���� */
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
      if(GPRS_AT_Test())                                                        //AT��������
        GPRSCMDNum++;
      break;
    }
    case 1:
    {
      if(GPRS_SetATE())                                                        //�رջ�������
      {
        AT24CXX_Read(GPRS_YorNFirst,&GPRSState,1);                                 //��ȡGPRS״̬
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
    case 2:                                                                     //���ù̶�������
    {
      if(GPRS_SetBT())
        GPRSCMDNum++;
      break;
    }
    case 3:                                                                     //ȷ��SIM��PIN���Ѿ�����
    {
      if(GPRS_SIMRight())
        GPRSCMDNum++;
      break;
    }  
    case 4:                                                                     //�ϱ��ƶ��豸����
    {
      if(GPRS_UpLoadError())
        GPRSCMDNum++;
      break;
    }
    case 5:                                                                     //��������ע���λ����Ϣ����������
    {
      if(GPRS_CREGSET())
        GPRSCMDNum++;
      break;
    }
    case 6:                                                                     //ȷ�������ɹ�
    {
      if(GPRS_FindNet())
        GPRSCMDNum++;
      break;
    }   
 
    case 7:                                                                     //��ѯGPRS�����Ƿ�ɹ�  ����ע��״̬
    {
      if(GPRS_Check())
        GPRSCMDNum++;
      break;
    }//////////////////////////////////////////////����Ϊ�������� 
    case 8:
    {
      num = GPRS_QIMUXCheck();
      if(num == 1)                                                           //�����Ӽ��
        GPRSCMDNum += 2;
      else if(num == 2)
      {
        GPRSCMDNum++;
      }
      break;
    }
    case 9:
    {
      if(GPRS_QIMUX())                                                           //����������
        GPRSCMDNum++;
      break;
    }
    case 10:                                                                     //�رճ�IP INITIAL״̬���κ�GPRS����
    {
      num = GPRS_QIDEACT();
      if(num == 1)
        GPRSCMDNum++;
      else if(num == 2)                                             //�رճ���ʧ��
        GPRSCMDNum = 3;
      break;
    }
    case 11:
    {
      if(GPRS_QIREGAPP())                                                       //�����������ý���� APN���û���������  �˺����벻�üӡ�
        GPRSCMDNum++;
      break;
    }
    case 12:
    {
      num = GPRS_QIACT();
      if(num == 1)                                                     //����GPRS��������  �����
        GPRSCMDNum++;
      else if(num == 2)                                               //�����ʧ��
        GPRSCMDNum = 7;
      break;
    }
    case 13:
    {
      num = GPRS_QISTAT();
      switch(num)                                                       //��ѯ����״̬
      {
        case 1:                                                         //���������
        {
          GPRSCMDNum++;
          break;
        }
        case 2:                                                         //TCP�ѽ���
        {
          GPRSCMDNum += 2;
          break;
        }
        case 3:                                                         //UDP�ѽ���
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
      if(num == 1)                                                //����TCP����
        GPRSCMDNum++;
      else if(num == 2)                                               //��������ʧ��
        GPRSCMDNum = 13;
      break;
    }
    case 15:
    {
      num = GPRS_QIOPRN_UDP();
      if(num == 1)                                                     //����UDP����
        GPRSCMDNum++;
      else if(num == 2)                                               //�����ʧ��
        GPRSCMDNum = 13;
      break;
    } 
    case 16:
    {
      if(GPRS_QINDISet())                                                       //�������ݽ��ղ����� ֱ�����������
        GPRSCMDNum++;
      break;
    }
    case 17:                                                                    //�������ӳɹ��ɹ�
    {
      GPRSCMDNum = 0;
      GPRSInitFinishFlag = 1;                                                //��ʼ����� 
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
      GPRS_AutoInit();                                                          //GPRS�Զ���������
    }
  }
  USART2_ReceiveData();                                                         //GPRS��������
}

void SensorCycleCollect(void)
{
  if(CollectionCount >= Sensor_UploadCycle*5)                                          //�ɼ�����    
  {
    CollectionCount = 0;
    read_temperature_and_humidity();
    bmp280GetData(&BMP280_Pressure,&BMP280_Tem,&BMP280_High);                                                //����ѹ
    CollectionFinishFlag = 1;
  }
}

/*��������*/
void GPRS_CollectionAndSendData(void)
{  
  SensorCycleCollect();
  
  if(GPRS_DataSendTim >= SensorCycle && CollectionFinishFlag && !USART2_UartReceiving && CompTimeFlag)                             //�ɼ����ݷ���                          //100ms
  {
    GPRS_DataSendTim = 0;  
    
    SensorDataWrite2Buf(GPRS);                                                  //���ɼ���������ͨ��������ʽ���ͳ�ȥ
    
    PDAMemoryDataWrite2Buf(GPRS);                                               //���ʹ洢PDA������
  }
}

uint8_t GPRSHaveMsgFlag = 0;                            //GPRS�����ݱ�־

void GPRS_HaveReceive(void)                             //�����ɹ�������д������ݼ��
{
  uint8_t test[255] = {0};
  if(USART2_UartDataFinishFlag)
  { 
    while(1)
    {
      if(GetServiceData("\r\n+RECEIVE: 0, ",GPRS_RxDataBuf))
      {
        ASICC2Char(GPRS_RxDataBuf,test,ReLength);                                                  //���㴮�ڹ۲�����
      
        Usart_SendNumString(USART1,test,2*ReLength);
        
        USART2_UartDataFinishFlag = 0;
        GPRSHaveMsgFlag = 1;
      }
      else
        break;                                                                  //��ֹ����ƽ̨����������������
    }
    if(ContrastData("\r\n> "))                                               //�ɹ�
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
      GPRS_Rssi = r_stRetData.ret1;                                             //��ȡ�ź�ǿ��
      USART2_UartDataFinishFlag = 0;
    }
    if(ContrastData("CLOSED"))                                                  //TCP���ӶϿ�
    {
      GPRSInitFinishFlag = 0;                                       
      GPRSCMDNum = 10;                                               //���»ص���ʼ��
      ConnectSurvival = 0;
      NetJoinFlag = 0;                                               //��������״̬�Ͽ�
      
      LCD12864_wstr(2,0,"GPRS�������ӶϿ�");
      NeedFrashScreeNetFlag = 1;
      CloseLedORScreeTim = 0;
      LCD12864_wstr(4,2," OFF");
    }
  }
}

/*��������*/
void GPRS_ReceiveData(void)
{
    GPRS_HaveReceive();
    if(GPRSHaveMsgFlag)                                  //�յ�����  �����ݵ����ȼ���
    {         
        GPRSHaveMsgFlag = 0;
        ThereHaveSM = 1;
        
        LED_BLUE_H();                                                           //�յ�NB����
        NeedCloseLedFlag = 1;
        CloseLedORScreeTim = 0;
        
        NetRxDataDeal(GPRS_RxDataBuf);                                          //����ָ���ִ��  ���忴"����ͨ��Э��"
    } 
    
    else if(TernimalACKTimCount > 10 && ThereHaveSM)                           //50ms ִ�в����Ļظ� ���忴"����ͨ��Э��"
    {
        TernimalACKTimCount = 0;
        TernimalACKCount++;
        WaitingACKCount++;
        
        if(NeedACK == 0x01)                                                       //��ҪACK
        {
          TernimalACKCount = 0;
          if(RTCTimeFlag)                                                             //��RTCʱ�ӽ����޸�
          {    
            RTCDataACK(GPRS);
          }
          else if(GPSTimeConfigFlag)                                                  //����GPS�ϴ�����
          {
            GPSTimConACK(GPRS,GPRS_RxDataBuf);
          }
          else if(SensorTimeConfigFlag)                                             //���ô������ɼ�����
          {
            SensorTimConACK(GPRS,GPRS_RxDataBuf);
          }
          else if(ICFlag)                                                       //IC��Ȩ
          {
            ICDataACK(GPRS);
          }
          else if(ClockFlag)                                                    //����������
          {
            SendLockOptionData(GPRS);
            
            LockDataAck(GPRS);                                                  //�������Ļ���
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
    if(NetJoinFlag == 2)                                                        //�����ɹ�
    {
      GPRS_CollectionAndSendData();                                                            //���ݲɼ��ͷ���
    }
#endif
    GPRS_ReceiveData();                                                         //���ݽ����봦��
    PDAOperationUpload();                                                       //PDA�����ϴ�
  }
}

void GPRSOption(void)
{
  if(GPRS)   
  {
    GPRS_Init();                                                                //GPRS��ʼ��������������ʼ��
    
    GPRS_RxTxData();                                                            //GPRS���ͺͽ������ݺ���
  }
}