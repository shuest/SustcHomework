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
    for(i=0;i<8;i++)                         // 循环8次
    {
        SPI_SCK_L;                           //拉低时钟
        if(byte&0x80) 
          SPI_MOSI_H;                                //若最到位为高，则输出高
        else          
          SPI_MOSI_L;                              //若最到位为低，则输出低
        byte <<= 1;                           // 低一位移位到最高位
        SPI_SCK_H;                           //拉高时钟
        Temp <<= 1;                           //数据左移
        if(READ_MISO) 
          Temp++;                       //若从从机接收到高电平，数据自加一
        SPI_SCK_L;                           //拉低时钟      
    }
    return (Temp);
}






























