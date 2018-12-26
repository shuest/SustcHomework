#ifndef _Coummand_H_
#define _Coummand_H_

#include "stm32f10x.h"



extern unsigned char Tab_Local_ID[8];    


extern float f_humi, f_temp;
extern unsigned char UART_Flage_Command;
extern unsigned char UM402_Flage_Command;

extern unsigned char Tab_Local_ID[8];            //����ID
extern unsigned char Tab_Target_ID[8];            //Ŀ��ID
extern unsigned char Target_ID_Fixed[8];            //Ŀ��ID�������ã�����ʹ��

extern unsigned char ElecLock_Target_ID_Fixed[8];   //������Ŀ��ID�������ã�����ʹ��
extern unsigned char ElecTrans_Target_ID_Fixed[8]; 		  //ͣ�������ǻ۽�ͨ��Ŀ��ID�������ã�����ʹ��

extern unsigned char PDA_Password[10];        //����PDA����
extern unsigned char PDA_Lora_Set[23];        //����LORA����
extern unsigned long Sensor_Time_Flage;      //�������ϴ�ʱ���־  0xAA �Զ��ɼ�  0X55�رղɼ��ϴ�
extern unsigned int Sensor_Time;            //�������ϴ�ʱ��
extern unsigned long GPS_Time;               //GPS�ϴ�ʱ��	
extern unsigned char GNSS_Mode;                  //GPS����ģʽ
extern unsigned char Sensor_Mode;                  //�������ɼ�ģʽ
extern unsigned char EEPROM_Data_Test[50];     //EEPROM ����
extern unsigned char Op_time[8];               //����ʱ��
extern unsigned char Battery_Voltage;         //��ص�ѹ��
extern unsigned char Battery_Tab;   
extern unsigned char Warn_Type; 
extern unsigned char Key_Val;                  //������־


//extern unsigned char EEPROM_Self_Test;       //EEPROM ������־     
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

#define Instruction_Temper_Update         	 ((uint8_t)0x40)  //0)��Ϣ�ɼ�

#define Instruction_Sealing         				 ((uint8_t)0x41)  //1)ʩ��ָ� 					0x41,��Ӧָ�� �� 0x42,
#define Instruction_Sealing_Response         ((uint8_t)0x42) 

#define Instruction_Unbind         				   ((uint8_t)0x43)  //2)���ָ� 					0x43,��Ӧָ�� �� 0x44,  
#define Instruction_Unbind_Response          ((uint8_t)0x44)

#define Instruction_Check_seal         			 ((uint8_t)0x45)  //3)���ָ� 					0x45,��Ӧָ�� �� 0x46,
#define Instruction_Check_seal_Response      ((uint8_t)0x46)

#define Instruction_Unwound_Alarm         	 ((uint8_t)0x47)  //4)��������� 					0x47,��Ӧָ�� �� 0x48,
#define Instruction_Unwound_Alarm_Response   ((uint8_t)0x48)

#define Instruction_Lifting_Lever         	 ((uint8_t)0x49)  //5)̧��ָ� 					0x49,��Ӧָ�� �� 0x4A;
#define Instruction_Lifting_Lever_Response   ((uint8_t)0x4A)


#define Instruction_PDA_Set_Time         	   ((uint8_t)0x63)  //6)����PDA��ǩ�ɼ�ʱ��	0x63,��Ӧָ�� �� 0x64;
#define Instruction_PDA_Set_Time_Response    ((uint8_t)0x64)

#define Instruction_PDA_Query_Time         	 ((uint8_t)0x65)  //7)��ѯPAD��ǩ�ɼ�ʱ��	0x65,��Ӧָ�� �� 0x66;
#define Instruction_PDA_Query_Time_Response  ((uint8_t)0x66)


#define Instruction_PDA_Set_Password         	((uint8_t)0x67)  //8)����PDA����      	0x67,��Ӧָ�� �� 0x68;
#define Instruction_PDA_Set_Password_Response ((uint8_t)0x68)

#define Instruction_PDA_Query_Password        ((uint8_t)0x69)  //9)��ѯPAD��ǩ�ɼ�ʱ��	0x69,��Ӧָ�� �� 0x6A;
#define Instruction_PDA_Query_Password_Response  ((uint8_t)0x6A)


#define Instruction_Sensor_Setting         	 ((uint8_t)0x6B)  //10)���ô������ɼ�ģʽ: 0x6B,��Ӧָ�� �� 0x6C, 
#define Instruction_Sensor_Setting_Response  ((uint8_t)0x6C)

#define Instruction_Query_Sensor_Setting         	 ((uint8_t)0x6D)  //11)��ѯ�������ɼ�ģʽ: 0x6D,��Ӧָ�� �� 0x6E, 
#define Instruction_Query_Sensor_Setting_Response  ((uint8_t)0x6E)

#define Instruction_Setting_ID         	     ((uint8_t)0x71)  //12)�����豸ID�ţ�			0x71,��Ӧָ�� �� 0x72,
#define Instruction_Setting_ID_Response      ((uint8_t)0x72)

#define Instruction_Setting_Target_ID         	     ((uint8_t)0x79)  //13)���ð���Ŀ���豸ID�ţ�			0x79,��Ӧָ�� �� 0x7A,
#define Instruction_Setting_Target_ID_Response      ((uint8_t)0x7A)

#define Instruction_Acquire_Target_ID         	     ((uint8_t)0x7B)  //14)���ð���Ŀ���豸ID�ţ�			0x79,��Ӧָ�� �� 0x7A,
#define Instruction_Acquire_Target_ID_Response      ((uint8_t)0x7C)


#define Instruction_Acquire_ID            	 ((uint8_t)0x73)  //15)��ȡ�豸ID�ţ�      0x73,��Ӧָ�� �� 0x74,
#define Instruction_Acquire_ID_Response      ((uint8_t)0x74)

#define Instruction_Setting_GPS           	 ((uint8_t)0x77)  //16)����GPSģ�������  0x77,��Ӧָ�� :  0x78,
#define Instruction_Setting_GPS_Response     ((uint8_t)0x78)

#define Instruction_PDA_Get_RTC         	   ((uint8_t)0x4b)  //6)??RTC??	0x4b,???? : 0x4c;
#define Instruction_PDA_Get_RTC_Response     ((uint8_t)0x4c)

#define Instruction_Setting_RTC           	 ((uint8_t)0x7D)  //17)����RTCʱ�䣺  0x7D,��Ӧָ�� :  0x7E,
#define Instruction_Setting_RTC_Response     ((uint8_t)0x7E)

#define Instruction_Setting_LORA_Channel                 ((uint8_t)0xA9)  //18)����LORAƵ�Σ�  0xA9,��Ӧָ�� :  0xAA, 
#define Instruction_Setting_LORA_Channel_Response        ((uint8_t)0xAA) 

#define Instruction_Acquire_LORA_Channel                 ((uint8_t)0xAA)  //19)��ѯLORAƵ�Σ�  0xAB,��Ӧָ�� :  0xAC, 
#define Instruction_Acquire_LORA_Channel_Response        ((uint8_t)0xAB) 

#define Instruction_Setting_ElcLock_TargetID         	 ((uint8_t)0xA1)  //17)���õ�����Ŀ��ID��  0xA1,��Ӧָ�� :  0xA2,
#define Instruction_Setting_ElcLock_TargetID_Response  ((uint8_t)0xA2)

#define Instruction_Acquire_ElcLock_TargetID         	 ((uint8_t)0xA3)  //17)��ȡ������Ŀ��ID��  0xA3,��Ӧָ�� :  0xA4,
#define Instruction_Acquire_ElcLock_TargetID_Response  ((uint8_t)0xA4)

#define Instruction_Setting_ElcTrans_TargetID         	((uint8_t)0xA5) //17)����ͣ�������ǻ۽�ͨ��Ŀ��ID��0xA5,��Ӧָ�� :  0xA6,
#define Instruction_Setting_ElcTrans_TargetID_Response  ((uint8_t)0xA6)

#define Instruction_Acquire_ElcTrans_TargetID         	((uint8_t)0xA7) //17)��ȡͣ�������ǻ۽�ͨ��Ŀ��ID��0xA7,��Ӧָ�� :  0xA8,
#define Instruction_Acquire_ElcTrans_TargetID_Response  ((uint8_t)0xA8)

	/*
   0x00  EEPROM ��ʼ��ַ
	 EEPROM �Ƿ��޸ı�־λ�洢λ�õ�ַ��2λ����0x01-0x02
	 �豸ID�洢��ַ��8λ��									 ��0x11-0x18
	 �������� �洢λ�ã�10λ��               ��0x21-0X2A
	 �������ϴ�ʱ�䣨2λ��                   ��0x2D
	 
	 LORA������Ϣ�洢 ��23λ��               ��0x30-0x46
	 GPS�ϴ�ʱ�䣨2λ��                      ��0x50
	 ��λоƬ����ģʽ                        ��0x=58;    01GPS��02BD   03˫ģ
	 ��ʪ�Ȳɼ�ģʽ 1byte                    : 0x5C;	
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
		  unsigned char 	flag;          //��ͷ 0X7B
			unsigned char   Protocol_ID;   //Э��ID 0x5A��ת�����  0X5B��������������
			unsigned char  	Equipment_ID[8];  //�豸ID 8���ֽ�  
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
	    unsigned char   TAB[30];
	    unsigned int    CRC16;
	
} UART_Command_TYPE; 

typedef struct
{ 
		  unsigned char 	flag;          //��ͷ 0X7B
	    unsigned char   Instruct;      //ָ������  
	    
			unsigned char   Local_ID[8];      //����ID
	    
			unsigned char   Target_ID[8];     //Ŀ��ID 8���ֽ�  
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
	    unsigned char   TAB[30];
	    unsigned int    CRC16;
	
} Um402_Command_TYPE; 





extern UART_Command_TYPE		UART_Command_Data;					//�ṹ����
extern Um402_Command_TYPE   Um402_Command_Data;         //�ṹ����


extern UART_Command_TYPE		UART_Command_Data_Sensor;					//�ṹ����
extern Um402_Command_TYPE   Um402_Command_Data_Sensor;         //�ṹ����



extern unsigned char UART_Data_1[50];
extern unsigned char UART_Data_2[50];



extern unsigned char Um402_Data_1[50];
extern unsigned char Um402_Data_2[50];


void Setting_LORA_Channel(void);
void Acquire_LORA_Channel(void);




#endif
