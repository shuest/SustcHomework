#ifndef _Coummand_H_
#define _Coummand_H_

#include "stm32f10x.h"
#include "CRC.h"
#include "usart.h"
#include "UART4.h"
#include "24cxx.h" 
#include "LCD12864.h"


extern unsigned char UART_Flage_Command;
extern unsigned char UM402_Flage_Command;

//extern unsigned char Tab_Local_ID[8];            //本机ID
//extern unsigned char PDA_Password_ID[10];        //本机PDA密码
//
//extern uint16_t Sensor_Time_Flage;      //传感器上传时间标志  0xAA 自动采集  0X55关闭采集上传
//extern uint16_t Sensor_Time;            //传感器上传时间
//extern unsigned long GPS_Time;               //GPS上传时间	
extern unsigned char EEPROM_Self_Test;       //EEPROM 操作标志     
/*
				bit 7                     bit 6                  bit 5
传感器上传时间设置标志       GPS上传时间设置标志       PDA采集模式标志
设置位1，未设置为0，     设置位1，未设置为0，      定时采集1，手动采集为0
*/
void Data_Dispose(void);


void PDA_Set_Time(void);
void PDA_Query_Time(void);
void Sensor_Setting(void);
void Query_Sensor_Setting(void);
void Setting_ID(void);
void Acquire_ID(void);
void PDA_Set_Password(void);
void PDA_Query_Password(void);


void  Data_UART(void);
void  Data_Um402(void);

#define Instruction_Environment              ((uint8_t)0x40)

#define Instruction_Sealing                  ((uint8_t)0x41)  //1)施封指令： 					0x41,响应指令 ： 0x42,
#define Instruction_Sealing_Response         ((uint8_t)0x42) 

#define Instruction_Unbind         	     ((uint8_t)0x43)  //2)解封指令： 					0x43,响应指令 ： 0x44,  
#define Instruction_Unbind_Response          ((uint8_t)0x44)

#define Instruction_Check_seal         	     ((uint8_t)0x45)  //3)验封指令： 					0x45,响应指令 ： 0x46,
#define Instruction_Check_seal_Response      ((uint8_t)0x46)

#define Instruction_Unwound_Alarm            ((uint8_t)0x47)  //4)解除报警： 					0x47,响应指令 ： 0x48,
#define Instruction_Unwound_Alarm_Response   ((uint8_t)0x48)

#define Instruction_Lifting_Lever            ((uint8_t)0x49)  //5)抬杆指令： 					0x49,响应指令 ： 0x4A;
#define Instruction_Lifting_Lever_Response   ((uint8_t)0x4A)

#define Instruction_PDA_Get_RTC             ((uint8_t)0x4b)  //PDA校验RTC时间	0x4B,响应指令 ： 0x4C;
#define Instruction_PDA_Get_RTC_Response    ((uint8_t)0x4c)

#define Instruction_PDA_Set_Time             ((uint8_t)0x63)  //6)设置PDA标签采集时间	0x63,响应指令 ： 0x64;
#define Instruction_PDA_Set_Time_Response    ((uint8_t)0x64)

#define Instruction_PDA_Query_Time           ((uint8_t)0x65)  //7)查询PAD标签采集时间	0x65,响应指令 ： 0x66;
#define Instruction_PDA_Query_Time_Response  ((uint8_t)0x66)


#define Instruction_PDA_Set_Password          ((uint8_t)0x67)  //8)设置PDA密码      	0x67,响应指令 ： 0x68;
#define Instruction_PDA_Set_Password_Response ((uint8_t)0x68)

#define Instruction_PDA_Query_Password        ((uint8_t)0x69)  //9)查询PAD标签采集时间	0x69,响应指令 ： 0x6A;
#define Instruction_PDA_Query_Password_Response  ((uint8_t)0x6A)


#define Instruction_Sensor_Setting           ((uint8_t)0x6B)  //10)设置传感器采集模式: 0x6B,响应指令 ： 0x6C, 
#define Instruction_Sensor_Setting_Response  ((uint8_t)0x6C)

#define Instruction_Query_Sensor_Setting           ((uint8_t)0x6D)  //11)查询传感器采集模式: 0x6D,响应指令 ： 0x6E, 
#define Instruction_Query_Sensor_Setting_Response  ((uint8_t)0x6E)

#define Instruction_Setting_ID         	     ((uint8_t)0x71)  //12)设置设备ID号：			0x71,响应指令 ： 0x72,
#define Instruction_Setting_ID_Response      ((uint8_t)0x72)

#define Instruction_Acquire_ID            	 ((uint8_t)0x73)  //13)获取设备ID号：      0x73,响应指令 ： 0x74,
#define Instruction_Acquire_ID_Response      ((uint8_t)0x74)

#define Instruction_Setting_GPS           	 ((uint8_t)0x75)  //14)设置GPS模块参数：  0x75,响应指令 :  0x76,
#define Instruction_Setting_GPS_Response     ((uint8_t)0x76)

#define Instruction_Acquire_GPS              ((uint8_t)0x77)  //15)查询GPS模块参数：  0x77,响应指令 :  0x78,
#define Instruction_Acquire_Response         ((uint8_t)0x78)

#define Instruction_Setting_RTC                 ((uint8_t)0x7D)  //16)设置RTC：  0x7D,响应指令 :  0x7E, 
#define Instruction_Setting_RTC_Response        ((uint8_t)0x7E)  

#define Instruction_Setting_NB_RM             ((uint8_t)0x89)  //17)设置NB扰码：  0x89,响应指令 :  0x8A, 
#define Instruction_Setting_NB_RM_Response    ((uint8_t)0x8A)  

#define Instruction_Setting_LORA_Channel                 ((uint8_t)0xA9)  //18)设置LORA频段：  0xA9,响应指令 :  0xAA, 
#define Instruction_Setting_LORA_Channel_Response        ((uint8_t)0xAA) 

#define Instruction_Acquire_LORA_Channel                 ((uint8_t)0xAB)  //19)查询LORA频段：  0xAB,响应指令 :  0xAC, 
#define Instruction_Acquire_LORA_Channel_Response        ((uint8_t)0xAC) 


typedef struct
{ 
  unsigned char   flag;          //包头 0X7B
  unsigned char   Protocol_ID;   //协议ID 0x5A（转发命令）  0X5B对自身设置命令   
  unsigned char   Equipment_ID[8];  //设备ID 8个字节 
  unsigned char   Instruction;   //指令 
  /*	
  1)施封指令： 					0x41,响应指令 ： 0x42,
  2)解封指令： 					0x43,响应指令 ： 0x44,
  3)验封指令： 					0x45,响应指令 ： 0x46,
  4)解除报警： 					0x47,响应指令 ： 0x48,
  5)抬杆指令： 					0x49,响应指令 ： 0x4A;
  6)传感器指令：			  0x40,响应指令 ： 无
  7)设置传感器采集模式: 0x6D,响应指令 ： 0x6E, 
  8)获取设备ID号：      0x67,响应指令 ： 0x68,
  9)设置设备ID号：			0x69,响应指令 ： 0x6A,
  10)设置GPS模块参数：   0x77,响应指令 :  0x78,

  */
  unsigned char 	Instruction_Length;   //指令长度
  unsigned char   TAB[20];
  unsigned int    CRC16;
} UART_Command_TYPE; 

typedef struct
{ 
  unsigned char 	flag;          //包头 0X7B
  unsigned char   Instruct;      //指令类型  

  unsigned char       Local_ID[8];      //本机ID

  unsigned char     	Target_ID[8];     //目标ID 8个字节  
  unsigned char 	Instruction;   //指令   
  /*	
  1)施封指令： 					0x41,响应指令 ： 0x42,
  2)解封指令： 					0x43,响应指令 ： 0x44,
  3)验封指令： 					0x45,响应指令 ： 0x46,
  4)解除报警： 					0x47,响应指令 ： 0x48,
  5)抬杆指令： 					0x49,响应指令 ： 0x4A;
  6)传感器指令：			  0x40,响应指令 ： 无
  7)设置传感器采集模式: 0x6D,响应指令 ： 0x6E, 
  8)获取设备ID号：      0x67,响应指令 ： 0x68,
  9)设置设备ID号：			0x69,响应指令 ： 0x6A,
  10)设置GPS模块参数：   0x77,响应指令 :  0x78,

  */
  unsigned char 	Instruction_Length;   //指令长度
  unsigned char   TAB[20];
  unsigned char    CRC16[2];
	
} Um402_Command_TYPE; 


extern unsigned char UART_Data_1[50];
extern unsigned char UART_Data_2[50];


extern unsigned char Um402_Data_1[50];
extern unsigned char Um402_Data_2[50];

/*add by guest*/
#define State_Warning 0x01                                                      //电量不足
#define State_Normal 0x00
/*add by guest*/

uint8_t Check_yORnReturn(uint8_t *data,uint8_t *ReturnData);
void OpenState_Check(uint8_t *data,uint8_t *ReturnData);
uint8_t ElecCheck(void);
void Acquire_GPS(void);
void Setting_GPS(void);
void Setting_Rm(void);
void Setting_RTC(void);

void Setting_LORA_Channel(void);
void Acquire_LORA_Channel(void);

extern unsigned char Tab_Local_ID[8];
extern uint8_t PDADataNumFull;
extern uint8_t PDACloseLockNum;                    //PDA在网络未连接时的关锁数
extern uint8_t PDACloseLockNumFull;
extern uint8_t PDAOpenLockNum;                     //PDA在网络未连接时的开锁数
extern uint8_t PDAOpenLockNumFull;

#endif
