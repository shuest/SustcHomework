#ifndef __MYIIC_H
#define __MYIIC_H
#include "IIC.h"
#include "stm32f10x.h"
  		   
//IO方向设置
//#define BMP_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define BMP_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

//IO操作函数	 
#define BMP_IIC_SCL    PBout(13)                                                   //SCL
#define BMP_IIC_SDA    PBout(12)                                                   //SDA	 
#define BMP_READ_SDA   PBin(12)                                                    //输入SDA 

//IIC所有操作函数
void BMP_IIC_Init(void);                                                           //初始化IIC的IO口				 
void BMP_IIC_Start(void);				                                           //发送IIC开始信号
void BMP_IIC_Stop(void);	  			                                           //发送IIC停止信号
void BMP_IIC_Send_Byte(u8 txd);			                                           //IIC发送一个字节
u8 BMP_IIC_Read_Byte(unsigned char ack);                                           //IIC读取一个字节
u8 BMP_IIC_Wait_Ack(void); 				                                           //IIC等待ACK信号
void BMP_IIC_Ack(void);					                                           //IIC发送ACK信号
void BMP_IIC_NAck(void);				                                           //IIC不发送ACK信号

void BMP_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 BMP_IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 BMP_iicDevReadByte(u8 devaddr,u8 addr);	                                       /*读一字节*/
void BMP_iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*写一字节*/
void BMP_iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*连续读取多个字节*/
void BMP_iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*连续写入多个字节*/

#endif
