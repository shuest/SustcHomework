#ifndef MMA7660_H
#define MMA7660_H

#include "stm32f10x.h"

/*************三轴加速度传感器模拟IIC协议引脚***********************/

#define SCL_H         GPIOB->BSRR = GPIO_Pin_3
#define SCL_L         GPIOB->BRR  = GPIO_Pin_3    
#define SDA_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR  = GPIO_Pin_4
#define SCL_read      GPIOB->IDR  & GPIO_Pin_3
#define SDA_read      GPIOB->IDR  & GPIO_Pin_4

void MMA7660_Init(void);
void MMA7660_GetXYZ(char *x, char *y, char *z);
void I2C_GPIO_Config(void);
void I2C_delay(void);
int I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
int I2C_WaitAck(void) ;
void I2C_SendByte(u8 SendByte);
u8 I2C_ReceiveByte(void);
#endif
