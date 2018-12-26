#ifndef __MYIIC_H
#define __MYIIC_H
#include "IIC.h"
#include "stm32f10x.h"
  		   
//IO��������
//#define BMP_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define BMP_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

//IO��������	 
#define BMP_IIC_SCL    PBout(13)                                                   //SCL
#define BMP_IIC_SDA    PBout(12)                                                   //SDA	 
#define BMP_READ_SDA   PBin(12)                                                    //����SDA 

//IIC���в�������
void BMP_IIC_Init(void);                                                           //��ʼ��IIC��IO��				 
void BMP_IIC_Start(void);				                                           //����IIC��ʼ�ź�
void BMP_IIC_Stop(void);	  			                                           //����IICֹͣ�ź�
void BMP_IIC_Send_Byte(u8 txd);			                                           //IIC����һ���ֽ�
u8 BMP_IIC_Read_Byte(unsigned char ack);                                           //IIC��ȡһ���ֽ�
u8 BMP_IIC_Wait_Ack(void); 				                                           //IIC�ȴ�ACK�ź�
void BMP_IIC_Ack(void);					                                           //IIC����ACK�ź�
void BMP_IIC_NAck(void);				                                           //IIC������ACK�ź�

void BMP_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 BMP_IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 BMP_iicDevReadByte(u8 devaddr,u8 addr);	                                       /*��һ�ֽ�*/
void BMP_iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*дһ�ֽ�*/
void BMP_iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*������ȡ����ֽ�*/
void BMP_iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*����д�����ֽ�*/

#endif
