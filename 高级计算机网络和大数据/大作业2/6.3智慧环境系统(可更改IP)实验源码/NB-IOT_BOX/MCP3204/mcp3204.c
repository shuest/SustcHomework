#include "mcp3204.h" 
#include "spi.h"
#include "delay.h"   
//Mini STM32������
//MCP3204 �������� 


//��ʼ��SPI FLASH��IO��
void SPI_MCP3204_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //MCP3024 CS 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_5);

	SPI1_Init();		   //��ʼ��SPI
}  

u16 SPI_MCP3204_ReadData(u8 channel)   
{  
	u8 byte=0,valh,vall;  
	u16 retval;
	byte=(channel <<6)& 0xc0;; 
	SPI_CS_L;                            //ʹ������   
	SPI1_RW_Byte(0x6);    //���Ͷ�ȡ״̬�Ĵ�������    
	valh=SPI1_RW_Byte(byte);             //��ȡһ���ֽ�  
	vall=SPI1_RW_Byte(0xff);
	SPI_CS_H;                            //ȡ��Ƭѡ       
	retval=valh & 0xf;
	retval =retval*256+vall;
	return retval;
}
