#ifndef _GPRS_H_
#define _GPRS_H_

#include "stm32f10x.h"
#include "stm32f10x_usart.h"

extern uint16_t GPRSTim;                               //GPRS��ѯʱ��
extern uint16_t GPRSTimCount;

extern unsigned char M26_AT_Return_Data; 
extern unsigned char M26_FrashFlag;
extern unsigned char M26_AT_Return_Data_flage;


#define         M26_OK                  0                               //OK
#define         STRING_ERR              1                               //��Ԥ�ڷ����ַ���
#define         CONNECT_ERR             2                               //��ģ�����Ӵ���
#define         QUERY_BAUD_ERR          3                               //�����ʴ���
#define         CLOSE_SOCKET_ERR        4
#define         CLOSE_CONTEXT_ERR       5
#define         SQ_ERR                  6                               //�źŲ�ѯ����
#define         REG_ERR                 7                               //����ע��ʧ��
#define         GREG_ERR                8                               //GPRS����ʧ��



#define         SET_CONTEXT_ERR         9
#define         ACT_CONTEXT_ERR         10   
#define         QUERY_ACT_ERR           11
#define         SET_SOCKET_ERR          12
#define         ENTER_DATA_MODE_ERR     13
#define         QUERY_SOCKET_ERR        14
#define         RESET_ERR               15
#define         DEFAULT_ERR             16
#define         QUERY_PIN_ERR           17                              //��SIM������SIM����PIN��
#define         BACK_TO_TRAN_ACCESS_MODE_ERR    18
#define         OVER_TIME               19
#define         Boot_up                 20


void M26_Data_Dispose(void);
void GPRS_RxTxData(void);
u8 GPRS_QISTAT(void);
u8 GPRS_QISTAT2(void);
void CheckGPRSConnect(void);

void GPRS_Init(void);
void GPRS_AutoInit(void);
void QuectelM26_SendCmd(unsigned char *Str);

extern void M26_HeartBeat(void);
extern uint8_t QuectelM26_Boot_up(void);

extern uint8_t QuectelM26_PowerUp(void);
extern uint8_t QuectelM26_PowerDown(void);
extern uint8_t QuectelM26_Init(void);                                //gsmģ���ʼ��
extern uint8_t QuectelM26_ConnectNet(void);                          //gsmģ����������
extern uint8_t QuectelM26_CloseNet(void);                            //gsm�ر���������
extern uint8_t QuectelM26_CheckNet(void);                            //��ѯgsm�����Ƿ��Ѿ���ͨ
extern uint8_t QuectelM26_ResetModule(void);                         //ģ�鸴λ
extern void QuectelM26_Demo(void);                                  //M26����Demo


uint8_t QuectelM26_SendData(uint8_t *buff, uint8_t num,uint8_t TcpOrUdp);

#define TCP 0
#define UDP 1

void GPRS_CollectionAndSendData(void);
void GPRS_ReceiveData(void);
void CloseGPRS(void);
void GPRSOption(void);

u8 GPRS_AT_Test(void);
u8 GPRS_SetATE(void);
u8 GPRS_SetBT(void);
u8 GPRS_SIMRight(void);
u8 GPRS_UpLoadError(void);
u8 GPRS_CREGSET(void);
u8 GPRS_FindNet(void);
u8 GPRS_Check(void);
u8 GPRS_QIMUXCheck(void);
u8 GPRS_QIMUX(void);
u8 GPRS_QIDEACT(void);
u8 GPRS_QIREGAPP(void);
u8 GPRS_QIACT(void);
u8 GPRS_QIOPRN_TCP(void);
u8 GPRS_QIOPRN_UDP(void);
u8 GPRS_QINDISet(void);

#endif
