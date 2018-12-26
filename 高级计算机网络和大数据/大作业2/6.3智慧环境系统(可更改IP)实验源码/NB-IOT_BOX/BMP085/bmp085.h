#ifndef BMP085_H
#define BMP085_H

#include "stm32f10x.h"
#include "gpio.h"

#define BMP085_ADDRESS        0xEE

  /*bmp085*/	
#define BMP_SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define BMP_SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define BMP_SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)

#define BMP_SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define BMP_SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_13)

extern	int32_t BMP085_Pressure;
extern  int32_t BMP085_Temp;


extern uint16_t t_point;

void Init_BMP085(void);
void Multiple_Read_BMP085(void);
int16_t BMP085_Read_2B(uint8_t addr);
uint32_t BMP085_Read_3B(uint8_t addr);
int16_t BMP085_Read_TEMP(void);
int32_t BMP085_Read_Pressure(void);


#endif
