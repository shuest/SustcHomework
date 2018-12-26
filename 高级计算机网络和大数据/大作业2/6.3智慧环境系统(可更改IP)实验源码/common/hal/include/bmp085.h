#ifndef BMP085_H
#define BMP085_h

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define BMP085_ADDRESS        0xEE

//#if CONFIG_SENSOR == SENSOR_Pressure
/*************大气压力传感器模拟IIC协议引脚***********************/


#define SCL_H         GPIOB->BSRR = GPIO_Pin_3
#define SCL_L         GPIOB->BRR  = GPIO_Pin_3    
#define SDA_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR  = GPIO_Pin_4
#define SCL_read      GPIOB->IDR  & GPIO_Pin_3
#define SDA_read      GPIOB->IDR  & GPIO_Pin_4


//#endif

void Init_BMP085(void);
void Multiple_Read_BMP085(long *press,uint16_t *temp);

#endif