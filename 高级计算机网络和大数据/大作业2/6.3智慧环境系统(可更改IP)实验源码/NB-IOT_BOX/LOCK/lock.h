#ifndef _LOCK_H_
#define _LOCK_H_

#include "stm32f10x.h"

#define LOCK_RCC RCC_APB2Periph_GPIOB
#define LORA_RCC RCC_APB2Periph_GPIOD
#define LOCK_GPIO GPIOB
#define LORA_GPIO GPIOD
#define LOCK_POWER_Pin GPIO_Pin_12
#define LORA_POWER_Pin GPIO_Pin_5
#define LOCK_Send1_Pin GPIO_Pin_13
#define LOCK_Send2_Pin GPIO_Pin_14

//Ëø×´Ì¬
#define LOCK_State_Empty 0
#define LOCK_State_Close 1
#define LOCK_State_Open 2
#define LOCK_State_Warning 3

//¸Ë×´Ì¬
#define Pole_State_Empty 0
#define Pole_State_Close 1
#define Pole_State_Open 2
#define Pole_State_Warning 3

void LOCK_GPIOConf(void);
void LOCK_Operation(uint8_t Op);

#endif
