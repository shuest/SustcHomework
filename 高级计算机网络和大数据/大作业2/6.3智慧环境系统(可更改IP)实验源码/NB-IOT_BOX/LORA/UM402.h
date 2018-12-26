#ifndef _UM402_H_
#define _UM402_H_

#include "stm32f10x.h"

extern unsigned int  UM402_Test_Flage;        //LORA���߱�־

extern unsigned char MU402_Config_Output[8];
extern unsigned char MU402_Config_Output_SN[9];


#define UM402_Mode_Fixed_length_atterns        ((uint8_t)0x01)  //����֡ģʽ
#define UM402_Mode_Field_testing               ((uint8_t)0x02)  //��ǿ���
#define UM402_Mode_Carrier_Tisten              ((uint8_t)0x04)  //�ز�֡��
#define UM402_Mode_Hardware_Addresses_Enable   ((uint8_t)0x08)  //Ӳ����ַʹ��
#define UM402_Mode_Low_Power_Consumption       ((uint8_t)0x20)  //�͹���ģʽ



#define UM402_Parity_No         ((uint8_t)0x00)
#define UM402_Parity_Even       ((uint8_t)0x01)
#define UM402_Parity_Odd        ((uint8_t)0x02)


typedef struct
{ 
		u32 Center_Frequency;      //����Ƶ��
		u16  Air_BaudRate;          //���в�����
		u8	Transmission_Ower;     //���书��
		u32	UART_BaudRate;		     //���ڲ�����
		u8  USART_Parity;          //����У�鷽ʽ
		u32	Wake_Cycle;            //��������
		u32	Trigger_Delay;         //������ʱ
		u32	Listen_Time;           //�ز�����ʱ��
		u8	TX_Lenth;              //���ݰ�����
		u32	Hardware_Address;      //Ӳ����ַ
		u32	Hardware_Model;        //ģʽ����

} UM402_InitDefine; 



typedef struct
{ 
		u32 TX_Center_Frequency;      //����Ƶ��
		u8  TX_Air_BaudRate;          //���в�����
		u8	TX_Transmission_Ower;     //���书��
		u8	TX_UART_BaudRate;		      //���ڲ�����
		u8  TX_USART_Parity;          //����У�鷽ʽ
		u8	TX_Wake_Cycle;            //��������
		u8	TX_Trigger_Delay;         //������ʱ
		u8	TX_Listen_Time;           //�ز�����ʱ��
		u8	TX_TX_Lenth;              //���ݰ�����
		u16	TX_Hardware_Address;      //Ӳ����ַ
		u8	TX_Hardware_Model;        //ģʽ����

} UM402_Output_InitDefine; 


extern UM402_InitDefine		UM402_InitStructure;					//�ṹ����

extern	UM402_Output_InitDefine UM402_Output_Initdata;    //�ṹ����

void Initial_UM402(void);
void  UM402_Configuration(void);

void LORA_TEST(void);
void LORA_DeviceInit(void);
void Parameter_Check(void);


#endif
