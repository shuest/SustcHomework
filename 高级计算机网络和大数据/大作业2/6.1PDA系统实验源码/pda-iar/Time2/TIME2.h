#ifndef TIME2_H
#define TIME2_H

#include "stm32f10x.h"
 
extern volatile u32 time2;
extern unsigned char Uart1_Recving;
extern unsigned char Uart1_Tick;
extern unsigned char Uart1_OV;


extern unsigned char Uart4_Recving;
extern unsigned char Uart4_Tick;
extern unsigned char Uart4_OV;
 
extern unsigned char Key_Tick;
extern unsigned char Key_Flag;

extern unsigned char Lcd_Flag;
extern unsigned char Lcd_Ready_Flag;
extern int Lcd_Tick;
extern int Lcd_Tick2;

//#define START_TIME2()  time2=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define START_TIME2()  {time2=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE);}

#define STOP_TIME2()   {TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);}

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);

#endif	/* TIME_TEST_H */
