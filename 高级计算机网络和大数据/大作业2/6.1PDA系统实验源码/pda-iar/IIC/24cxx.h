#ifndef __24CXX_H
#define __24CXX_H
#include "IIC.h"    
	 
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
#define EE_TYPE AT24C256						//使用的是24c256，所以定义EE_TYPE为AT24C256

#define E2PROM_OK 	1
#define E2PROM_ERR	-1

#define MODE_R 		0X55
#define MODE_W 		0XAE
//--------------EEPROM AT24C256 ------------------------------
#define Test_ADDR					0x0000 //1B
	




					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);					
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);	
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);				
// u32 *AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   
void AT24CXX_Init(void); 
void E2PROM_OP(u8 *datbuf,u16 address,u8 num,u8 mode);
s8 AT24CXX_Check(void);
#endif














