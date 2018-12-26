#ifndef TIME4_H
#define TIME4_H

#include "stm32f10x.h"

extern volatile u32 time4;
extern volatile u32 Adc_time;
 
#define START_TIME4()  {time4=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);TIM_Cmd(TIM4, ENABLE);}
#define STOP_TIME4()   {TIM_Cmd(TIM4, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);}

void TIM4_NVIC_Configuration(void);
void TIM4_Configuration(void);

#endif
