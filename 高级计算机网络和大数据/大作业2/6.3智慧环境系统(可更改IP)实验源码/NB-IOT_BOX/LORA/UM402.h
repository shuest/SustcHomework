#ifndef _UM402_H_
#define _UM402_H_

#include "stm32f10x.h"

extern unsigned int  UM402_Test_Flage;        //LORA在线标志

extern unsigned char MU402_Config_Output[8];
extern unsigned char MU402_Config_Output_SN[9];


#define UM402_Mode_Fixed_length_atterns        ((uint8_t)0x01)  //定长帧模式
#define UM402_Mode_Field_testing               ((uint8_t)0x02)  //场强检测
#define UM402_Mode_Carrier_Tisten              ((uint8_t)0x04)  //载波帧听
#define UM402_Mode_Hardware_Addresses_Enable   ((uint8_t)0x08)  //硬件地址使能
#define UM402_Mode_Low_Power_Consumption       ((uint8_t)0x20)  //低功耗模式



#define UM402_Parity_No         ((uint8_t)0x00)
#define UM402_Parity_Even       ((uint8_t)0x01)
#define UM402_Parity_Odd        ((uint8_t)0x02)


typedef struct
{ 
		u32 Center_Frequency;      //中心频率
		u16  Air_BaudRate;          //空中波特率
		u8	Transmission_Ower;     //发射功率
		u32	UART_BaudRate;		     //串口波特率
		u8  USART_Parity;          //串口校验方式
		u32	Wake_Cycle;            //唤醒周期
		u32	Trigger_Delay;         //触发延时
		u32	Listen_Time;           //载波侦听时间
		u8	TX_Lenth;              //数据包长度
		u32	Hardware_Address;      //硬件地址
		u32	Hardware_Model;        //模式设置

} UM402_InitDefine; 



typedef struct
{ 
		u32 TX_Center_Frequency;      //中心频率
		u8  TX_Air_BaudRate;          //空中波特率
		u8	TX_Transmission_Ower;     //发射功率
		u8	TX_UART_BaudRate;		      //串口波特率
		u8  TX_USART_Parity;          //串口校验方式
		u8	TX_Wake_Cycle;            //唤醒周期
		u8	TX_Trigger_Delay;         //触发延时
		u8	TX_Listen_Time;           //载波侦听时间
		u8	TX_TX_Lenth;              //数据包长度
		u16	TX_Hardware_Address;      //硬件地址
		u8	TX_Hardware_Model;        //模式设置

} UM402_Output_InitDefine; 


extern UM402_InitDefine		UM402_InitStructure;					//结构定义

extern	UM402_Output_InitDefine UM402_Output_Initdata;    //结构定义

void Initial_UM402(void);
void  UM402_Configuration(void);

void LORA_TEST(void);
void LORA_DeviceInit(void);
void Parameter_Check(void);


#endif
