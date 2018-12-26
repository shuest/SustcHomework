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

//extern unsigned char Tab_Local_ID[8];            //����ID
//extern unsigned char PDA_Password_ID[10];        //����PDA����
//
//extern uint16_t Sensor_Time_Flage;      //�������ϴ�ʱ���־  0xAA �Զ��ɼ�  0X55�رղɼ��ϴ�
//extern uint16_t Sensor_Time;            //�������ϴ�ʱ��
//extern unsigned long GPS_Time;               //GPS�ϴ�ʱ��	
extern unsigned char EEPROM_Self_Test;       //EEPROM ������־     
/*
				bit 7                     bit 6                  bit 5
�������ϴ�ʱ�����ñ�־       GPS�ϴ�ʱ�����ñ�־       PDA�ɼ�ģʽ��־
����λ1��δ����Ϊ0��     ����λ1��δ����Ϊ0��      ��ʱ�ɼ�1���ֶ��ɼ�Ϊ0
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

#define Instruction_Sealing                  ((uint8_t)0x41)  //1)ʩ��ָ� 					0x41,��Ӧָ�� �� 0x42,
#define Instruction_Sealing_Response         ((uint8_t)0x42) 

#define Instruction_Unbind         	     ((uint8_t)0x43)  //2)���ָ� 					0x43,��Ӧָ�� �� 0x44,  
#define Instruction_Unbind_Response          ((uint8_t)0x44)

#define Instruction_Check_seal         	     ((uint8_t)0x45)  //3)���ָ� 					0x45,��Ӧָ�� �� 0x46,
#define Instruction_Check_seal_Response      ((uint8_t)0x46)

#define Instruction_Unwound_Alarm            ((uint8_t)0x47)  //4)��������� 					0x47,��Ӧָ�� �� 0x48,
#define Instruction_Unwound_Alarm_Response   ((uint8_t)0x48)

#define Instruction_Lifting_Lever            ((uint8_t)0x49)  //5)̧��ָ� 					0x49,��Ӧָ�� �� 0x4A;
#define Instruction_Lifting_Lever_Response   ((uint8_t)0x4A)

#define Instruction_PDA_Get_RTC             ((uint8_t)0x4b)  //PDAУ��RTCʱ��	0x4B,��Ӧָ�� �� 0x4C;
#define Instruction_PDA_Get_RTC_Response    ((uint8_t)0x4c)

#define Instruction_PDA_Set_Time             ((uint8_t)0x63)  //6)����PDA��ǩ�ɼ�ʱ��	0x63,��Ӧָ�� �� 0x64;
#define Instruction_PDA_Set_Time_Response    ((uint8_t)0x64)

#define Instruction_PDA_Query_Time           ((uint8_t)0x65)  //7)��ѯPAD��ǩ�ɼ�ʱ��	0x65,��Ӧָ�� �� 0x66;
#define Instruction_PDA_Query_Time_Response  ((uint8_t)0x66)


#define Instruction_PDA_Set_Password          ((uint8_t)0x67)  //8)����PDA����      	0x67,��Ӧָ�� �� 0x68;
#define Instruction_PDA_Set_Password_Response ((uint8_t)0x68)

#define Instruction_PDA_Query_Password        ((uint8_t)0x69)  //9)��ѯPAD��ǩ�ɼ�ʱ��	0x69,��Ӧָ�� �� 0x6A;
#define Instruction_PDA_Query_Password_Response  ((uint8_t)0x6A)


#define Instruction_Sensor_Setting           ((uint8_t)0x6B)  //10)���ô������ɼ�ģʽ: 0x6B,��Ӧָ�� �� 0x6C, 
#define Instruction_Sensor_Setting_Response  ((uint8_t)0x6C)

#define Instruction_Query_Sensor_Setting           ((uint8_t)0x6D)  //11)��ѯ�������ɼ�ģʽ: 0x6D,��Ӧָ�� �� 0x6E, 
#define Instruction_Query_Sensor_Setting_Response  ((uint8_t)0x6E)

#define Instruction_Setting_ID         	     ((uint8_t)0x71)  //12)�����豸ID�ţ�			0x71,��Ӧָ�� �� 0x72,
#define Instruction_Setting_ID_Response      ((uint8_t)0x72)

#define Instruction_Acquire_ID            	 ((uint8_t)0x73)  //13)��ȡ�豸ID�ţ�      0x73,��Ӧָ�� �� 0x74,
#define Instruction_Acquire_ID_Response      ((uint8_t)0x74)

#define Instruction_Setting_GPS           	 ((uint8_t)0x75)  //14)����GPSģ�������  0x75,��Ӧָ�� :  0x76,
#define Instruction_Setting_GPS_Response     ((uint8_t)0x76)

#define Instruction_Acquire_GPS              ((uint8_t)0x77)  //15)��ѯGPSģ�������  0x77,��Ӧָ�� :  0x78,
#define Instruction_Acquire_Response         ((uint8_t)0x78)

#define Instruction_Setting_RTC                 ((uint8_t)0x7D)  //16)����RTC��  0x7D,��Ӧָ�� :  0x7E, 
#define Instruction_Setting_RTC_Response        ((uint8_t)0x7E)  

#define Instruction_Setting_NB_RM             ((uint8_t)0x89)  //17)����NB���룺  0x89,��Ӧָ�� :  0x8A, 
#define Instruction_Setting_NB_RM_Response    ((uint8_t)0x8A)  

#define Instruction_Setting_LORA_Channel                 ((uint8_t)0xA9)  //18)����LORAƵ�Σ�  0xA9,��Ӧָ�� :  0xAA, 
#define Instruction_Setting_LORA_Channel_Response        ((uint8_t)0xAA) 

#define Instruction_Acquire_LORA_Channel                 ((uint8_t)0xAB)  //19)��ѯLORAƵ�Σ�  0xAB,��Ӧָ�� :  0xAC, 
#define Instruction_Acquire_LORA_Channel_Response        ((uint8_t)0xAC) 


typedef struct
{ 
  unsigned char   flag;          //��ͷ 0X7B
  unsigned char   Protocol_ID;   //Э��ID 0x5A��ת�����  0X5B��������������   
  unsigned char   Equipment_ID[8];  //�豸ID 8���ֽ� 
  unsigned char   Instruction;   //ָ�� 
  /*	
  1)ʩ��ָ� 					0x41,��Ӧָ�� �� 0x42,
  2)���ָ� 					0x43,��Ӧָ�� �� 0x44,
  3)���ָ� 					0x45,��Ӧָ�� �� 0x46,
  4)��������� 					0x47,��Ӧָ�� �� 0x48,
  5)̧��ָ� 					0x49,��Ӧָ�� �� 0x4A;
  6)������ָ�			  0x40,��Ӧָ�� �� ��
  7)���ô������ɼ�ģʽ: 0x6D,��Ӧָ�� �� 0x6E, 
  8)��ȡ�豸ID�ţ�      0x67,��Ӧָ�� �� 0x68,
  9)�����豸ID�ţ�			0x69,��Ӧָ�� �� 0x6A,
  10)����GPSģ�������   0x77,��Ӧָ�� :  0x78,

  */
  unsigned char 	Instruction_Length;   //ָ���
  unsigned char   TAB[20];
  unsigned int    CRC16;
} UART_Command_TYPE; 

typedef struct
{ 
  unsigned char 	flag;          //��ͷ 0X7B
  unsigned char   Instruct;      //ָ������  

  unsigned char       Local_ID[8];      //����ID

  unsigned char     	Target_ID[8];     //Ŀ��ID 8���ֽ�  
  unsigned char 	Instruction;   //ָ��   
  /*	
  1)ʩ��ָ� 					0x41,��Ӧָ�� �� 0x42,
  2)���ָ� 					0x43,��Ӧָ�� �� 0x44,
  3)���ָ� 					0x45,��Ӧָ�� �� 0x46,
  4)��������� 					0x47,��Ӧָ�� �� 0x48,
  5)̧��ָ� 					0x49,��Ӧָ�� �� 0x4A;
  6)������ָ�			  0x40,��Ӧָ�� �� ��
  7)���ô������ɼ�ģʽ: 0x6D,��Ӧָ�� �� 0x6E, 
  8)��ȡ�豸ID�ţ�      0x67,��Ӧָ�� �� 0x68,
  9)�����豸ID�ţ�			0x69,��Ӧָ�� �� 0x6A,
  10)����GPSģ�������   0x77,��Ӧָ�� :  0x78,

  */
  unsigned char 	Instruction_Length;   //ָ���
  unsigned char   TAB[20];
  unsigned char    CRC16[2];
	
} Um402_Command_TYPE; 


extern unsigned char UART_Data_1[50];
extern unsigned char UART_Data_2[50];


extern unsigned char Um402_Data_1[50];
extern unsigned char Um402_Data_2[50];

/*add by guest*/
#define State_Warning 0x01                                                      //��������
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
extern uint8_t PDACloseLockNum;                    //PDA������δ����ʱ�Ĺ�����
extern uint8_t PDACloseLockNumFull;
extern uint8_t PDAOpenLockNum;                     //PDA������δ����ʱ�Ŀ�����
extern uint8_t PDAOpenLockNumFull;

#endif
