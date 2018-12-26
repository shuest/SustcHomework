#ifndef TIME3_H
#define TIME3_H

#include "stm32f10x.h"

extern volatile u32 time3;

 
#define START_TIME3()  time3=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME3()   TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)

void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);

#endif
