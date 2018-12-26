#ifndef _Coummand_H_
#define _Coummand_H_

#include "stm32f10x.h"



extern unsigned char Tab_Local_ID[8];    


extern float f_humi, f_temp;
extern unsigned char UART_Flage_Command;
extern unsigned char UM402_Flage_Command;

extern unsigned char Tab_Local_ID[8];            //本机ID
extern unsigned char Tab_Target_ID[8];            //目标ID
extern unsigned char Target_ID_Fixed[8];            //目标ID串口配置，按键使用

extern unsigned char ElecLock_Target_ID_Fixed[8];   //电子锁目标ID串口配置，按键使用
extern unsigned char ElecTrans_Target_ID_Fixed[8]; 		  //停车场（智慧交通）目标ID串口配置，按键使用

extern unsigned char PDA_Password[10];        //本机PDA密码
extern unsigned char PDA_Lora_Set[23];        //设置LORA参数
extern unsigned long Sensor_Time_Flage;      //传感器上传时间标志  0xAA 自动采集  0X55关闭采集上传
extern unsigned int Sensor_Time;            //传感器上传时间
extern unsigned long GPS_Time;               //GPS上传时间	
extern unsigned char GNSS_Mode;                  //GPS工作模式
extern unsigned char Sensor_Mode;                  //传感器采集模式
extern unsigned char EEPROM_Data_Test[50];     //EEPROM 数据
extern unsigned char Op_time[8];               //操作时间
extern unsigned char Battery_Voltage;         //电池电压检
extern unsigned char Battery_Tab;   
extern unsigned char Warn_Type; 
extern unsigned char Key_Val;                  //按键标志


//extern unsigned char EEPROM_Self_Test;       //EEPROM 操作标志     
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
void Setting_Target_ID(void);
void Acquire_Target_ID(void);
void PDA_Set_Password(void);
void PDA_Query_Password(void);
void Setting_GPS(void);

void Setting_ElcLock_TargetID(void);
void Acquire_ElcLock_TargetID(void);
void Setting_ElcTrans_TargetID(void);
void Acquire_ElcTrans_TargetID(void);			

void Display_Time(void);
void Sensor_Data_Update(void);
void SHT1X_Read_Data(void);
//unsigned char DAT_Vol(void);
// void Lock(void);
// void UnLock(void);
// void Lock_Check(void);
// void Remove_Alarm(void);
// void Lift_Rod(void);
void Com_To_Rf(void);
void Key_To_Um402(void);
void Self_Check(void);
void Param_Get(void);
int memcmp_new(const void *s1, const void *s2, size_t n);
void Uart1_Txbuf_Get(void);
void Setting_RTC(void);
void UM402_Analysis(void);

#define Instruction_Temper_Update         	 ((uint8_t)0x40)  //0)信息采集

#define Instruction_Sealing         				 ((uint8_t)0x41)  //1)施封指令： 					0x41,响应指令 ： 0x42,
#define Instruction_Sealing_Response         ((uint8_t)0x42) 

#define Instruction_Unbind         				   ((uint8_t)0x43)  //2)解封指令： 					0x43,响应指令 ： 0x44,  
#define Instruction_Unbind_Response          ((uint8_t)0x44)

#define Instruction_Check_seal         			 ((uint8_t)0x45)  //3)验封指令： 					0x45,响应指令 ： 0x46,
#define Instruction_Check_seal_Response      ((uint8_t)0x46)

#define Instruction_Unwound_Alarm         	 ((uint8_t)0x47)  //4)解除报警： 					0x47,响应指令 ： 0x48,
#define Instruction_Unwound_Alarm_Response   ((uint8_t)0x48)

#define Instruction_Lifting_Lever         	 ((uint8_t)0x49)  //5)抬杆指令： 					0x49,响应指令 ： 0x4A;
#define Instruction_Lifting_Lever_Response   ((uint8_t)0x4A)


#define Instruction_PDA_Set_Time         	   ((uint8_t)0x63)  //6)设置PDA标签采集时间	0x63,响应指令 ： 0x64;
#define Instruction_PDA_Set_Time_Response    ((uint8_t)0x64)

#define Instruction_PDA_Query_Time         	 ((uint8_t)0x65)  //7)查询PAD标签采集时间	0x65,响应指令 ： 0x66;
#define Instruction_PDA_Query_Time_Response  ((uint8_t)0x66)


#define Instruction_PDA_Set_Password         	((uint8_t)0x67)  //8)设置PDA密码      	0x67,响应指令 ： 0x68;
#define Instruction_PDA_Set_Password_Response ((uint8_t)0x68)

#define Instruction_PDA_Query_Password        ((uint8_t)0x69)  //9)查询PAD标签采集时间	0x69,响应指令 ： 0x6A;
#define Instruction_PDA_Query_Password_Response  ((uint8_t)0x6A)


#define Instruction_Sensor_Setting         	 ((uint8_t)0x6B)  //10)设置传感器采集模式: 0x6B,响应指令 ： 0x6C, 
#define Instruction_Sensor_Setting_Response  ((uint8_t)0x6C)

#define Instruction_Query_Sensor_Setting         	 ((uint8_t)0x6D)  //11)查询传感器采集模式: 0x6D,响应指令 ： 0x6E, 
#define Instruction_Query_Sensor_Setting_Response  ((uint8_t)0x6E)

#define Instruction_Setting_ID         	     ((uint8_t)0x71)  //12)设置设备ID号：			0x71,响应指令 ： 0x72,
#define Instruction_Setting_ID_Response      ((uint8_t)0x72)

#define Instruction_Setting_Target_ID         	     ((uint8_t)0x79)  //13)设置按键目标设备ID号：			0x79,响应指令 ： 0x7A,
#define Instruction_Setting_Target_ID_Response      ((uint8_t)0x7A)

#define Instruction_Acquire_Target_ID         	     ((uint8_t)0x7B)  //14)设置按键目标设备ID号：			0x79,响应指令 ： 0x7A,
#define Instruction_Acquire_Target_ID_Response      ((uint8_t)0x7C)


#define Instruction_Acquire_ID            	 ((uint8_t)0x73)  //15)获取设备ID号：      0x73,响应指令 ： 0x74,
#define Instruction_Acquire_ID_Response      ((uint8_t)0x74)

#define Instruction_Setting_GPS           	 ((uint8_t)0x77)  //16)设置GPS模块参数：  0x77,响应指令 :  0x78,
#define Instruction_Setting_GPS_Response     ((uint8_t)0x78)

#define Instruction_PDA_Get_RTC         	   ((uint8_t)0x4b)  //6)??RTC??	0x4b,???? : 0x4c;
#define Instruction_PDA_Get_RTC_Response     ((uint8_t)0x4c)

#define Instruction_Setting_RTC           	 ((uint8_t)0x7D)  //17)设置RTC时间：  0x7D,响应指令 :  0x7E,
#define Instruction_Setting_RTC_Response     ((uint8_t)0x7E)

#define Instruction_Setting_LORA_Channel                 ((uint8_t)0xA9)  //18)设置LORA频段：  0xA9,响应指令 :  0xAA, 
#define Instruction_Setting_LORA_Channel_Response        ((uint8_t)0xAA) 

#define Instruction_Acquire_LORA_Channel                 ((uint8_t)0xAA)  //19)查询LORA频段：  0xAB,响应指令 :  0xAC, 
#define Instruction_Acquire_LORA_Channel_Response        ((uint8_t)0xAB) 

#define Instruction_Setting_ElcLock_TargetID         	 ((uint8_t)0xA1)  //17)设置电子锁目标ID：  0xA1,响应指令 :  0xA2,
#define Instruction_Setting_ElcLock_TargetID_Response  ((uint8_t)0xA2)

#define Instruction_Acquire_ElcLock_TargetID         	 ((uint8_t)0xA3)  //17)获取电子锁目标ID：  0xA3,响应指令 :  0xA4,
#define Instruction_Acquire_ElcLock_TargetID_Response  ((uint8_t)0xA4)

#define Instruction_Setting_ElcTrans_TargetID         	((uint8_t)0xA5) //17)设置停车场（智慧交通）目标ID：0xA5,响应指令 :  0xA6,
#define Instruction_Setting_ElcTrans_TargetID_Response  ((uint8_t)0xA6)

#define Instruction_Acquire_ElcTrans_TargetID         	((uint8_t)0xA7) //17)获取停车场（智慧交通）目标ID：0xA7,响应指令 :  0xA8,
#define Instruction_Acquire_ElcTrans_TargetID_Response  ((uint8_t)0xA8)

	/*
   0x00  EEPROM 起始地址
	 EEPROM 是否修改标志位存储位置地址（2位）：0x01-0x02
	 设备ID存储地址（8位）									 ：0x11-0x18
	 操作密码 存储位置（10位）               ：0x21-0X2A
	 传感器上传时间（2位）                   ：0x2D
	 
	 LORA配置信息存储 （23位）               ：0x30-0x46
	 GPS上传时间（2位）                      ：0x50
	 定位芯片工作模式                        ：0x=58;    01GPS，02BD   03双模
	 温湿度采集模式 1byte                    : 0x5C;	
*/

#define Reset_Flag_Addr       ((uint8_t)0x01)    //2byte
#define Equipment_ID_Arr      ((uint8_t)0x11)    //8byte
#define PassWord_Arr          ((uint8_t)0x21)    //10byte
#define Sensor_Time_Adrr      ((uint8_t)0x2D)    //2byte
#define Lora_Set_Adrr         ((uint8_t)0x30)    //23byte
#define GPS_Time_Adrr         ((uint8_t)0x50)    //2byte
#define GNSS_Mode_Addr        ((uint8_t)0x58)    //1byte
#define Sensor_Mode_Adrr      ((uint8_t)0x5c)    //1byte
#define Target_ID_Fixed_Arr   ((uint8_t)0x61)    //8byte

#define ElecLock_Target_ID_Fixed_Arr    ((uint8_t)0x73)    //8byte
#define ElecTrans_Target_ID_Fixed_Arr   ((uint8_t)0x7D)    //8byte

typedef struct
{ 
		  unsigned char 	flag;          //包头 0X7B
			unsigned char   Protocol_ID;   //协议ID 0x5A（转发命令）  0X5B对自身设置命令
			unsigned char  	Equipment_ID[8];  //设备ID 8个字节  
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
	    unsigned char   TAB[30];
	    unsigned int    CRC16;
	
} UART_Command_TYPE; 

typedef struct
{ 
		  unsigned char 	flag;          //包头 0X7B
	    unsigned char   Instruct;      //指令类型  
	    
			unsigned char   Local_ID[8];      //本机ID
	    
			unsigned char   Target_ID[8];     //目标ID 8个字节  
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
	    unsigned char   TAB[30];
	    unsigned int    CRC16;
	
} Um402_Command_TYPE; 





extern UART_Command_TYPE		UART_Command_Data;					//结构定义
extern Um402_Command_TYPE   Um402_Command_Data;         //结构定义


extern UART_Command_TYPE		UART_Command_Data_Sensor;					//结构定义
extern Um402_Command_TYPE   Um402_Command_Data_Sensor;         //结构定义



extern unsigned char UART_Data_1[50];
extern unsigned char UART_Data_2[50];



extern unsigned char Um402_Data_1[50];
extern unsigned char Um402_Data_2[50];


void Setting_LORA_Channel(void);
void Acquire_LORA_Channel(void);




#endif
