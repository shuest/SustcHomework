#ifndef SHT10_H
#define SHT10_H

#include "delay.h"
#include "stm32f10x.h"


#define SDA_H()         GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define SDA_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define SCK_H()         GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define SCK_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_15)
//读SDA数据
#define SDA_R()         GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)


#define noACK 0         //无应答
#define ACK   1         //应答

#define STATUS_REG_W    0x06   //000   0011    0
#define STATUS_REG_R    0x07   //000   0011    1
#define MEASURE_TEMP    0x03   //000   0001    1
#define MEASURE_HUMI    0x05   //000   0010    1
#define RESET           0x1e   //000   1111    0

enum {TEMP,HUMI};


//IIC所有操作函数
void SHT10_IOConfig(void);
void SDA_In(void);
void SDA_Out(void);
u8 WriteByte(u8 value);
u8 ReadByte (u8 Ack);
void Start (void);
void ConReset (void);
u8 SoftReset (void);
u8 Measure(u16 *pValue, u8 *pCheckSum, u8 mode);
void Cal(u16 Temp, u16 Hum, float* pTempValue, float* pHumValue);
void RCC_Config(void);
void SHT10_Delay(void);
void read_temperature_and_humidity (void);

#endif

