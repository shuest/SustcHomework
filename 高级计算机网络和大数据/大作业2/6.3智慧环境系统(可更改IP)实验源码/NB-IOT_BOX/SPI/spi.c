#include "spi.h"	  

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
        GPIO_InitStructure.GPIO_Pin = SPI_MOSI;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(SPI_Port,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = SPI_SCK;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = SPI_MISO;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(SPI_Port,&GPIO_InitStructure);	 
}   

u8 SPI1_RW_Byte(u8 byte)
{
    u8 i,Temp=0;
    for(i=0;i<8;i++)                         // ѭ��8��
    {
        SPI_SCK_L;                           //����ʱ��
        if(byte&0x80) 
          SPI_MOSI_H;                                //���λΪ�ߣ��������
        else          
          SPI_MOSI_L;                              //���λΪ�ͣ��������
        byte <<= 1;                           // ��һλ��λ�����λ
        SPI_SCK_H;                           //����ʱ��
        Temp <<= 1;                           //��������
        if(READ_MISO) 
          Temp++;                       //���Ӵӻ����յ��ߵ�ƽ�������Լ�һ
        SPI_SCK_L;                           //����ʱ��      
    }
    return (Temp);
}






























