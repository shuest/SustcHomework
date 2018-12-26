#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define SPI_MOSI_H GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define SPI_MOSI_L GPIO_ResetBits(GPIOB,GPIO_Pin_4)

#define SPI_CS_H GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define SPI_CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define READ_MISO GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)

#define SPI_SCK_H GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define SPI_SCK_L GPIO_ResetBits(GPIOA,GPIO_Pin_15)

#define SPI_MOSI GPIO_Pin_4
#define SPI_MISO GPIO_Pin_3
#define SPI_SCK GPIO_Pin_15

#define SPI_Port GPIOB

void SPI1_Init(void);
u8 SPI1_RW_Byte(u8 byte);
		 
#endif

