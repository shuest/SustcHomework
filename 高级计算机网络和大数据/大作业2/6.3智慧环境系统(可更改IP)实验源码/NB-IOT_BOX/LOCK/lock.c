#include "lock.h"
#include "delay.h"
#include "gpio.h"

extern uint8_t LockRunTim;
extern uint8_t UpliftPoleTimCount;
extern uint8_t LockChangeTim;

uint8_t NeedChangeLockState = 0;

void LOCK_GPIOConf(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LOCK_RCC, ENABLE);  
    
    /* 配置LOCK POWER为推挽输出模式*/
    GPIO_InitStructure.GPIO_Pin = LOCK_POWER_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LOCK_GPIO, &GPIO_InitStructure);
    
    /* 配置Send1为推挽输出模式*/
    GPIO_InitStructure.GPIO_Pin = LOCK_Send1_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LOCK_GPIO, &GPIO_InitStructure);
    
    /* 配置Send2为推挽输出模式*/
    GPIO_InitStructure.GPIO_Pin = LOCK_Send2_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LOCK_GPIO, &GPIO_InitStructure);
      
    GPIO_SetBits(LOCK_GPIO,LOCK_POWER_Pin);                                     //使能LOCK
}

uint8_t count = 0;

void UpliftPole_Operation(void)
{
  LED_BLUE_H();
  if(!count)
  {
    count = 1;
    UpliftPoleTimCount = 0;
  }
  if(UpliftPoleTimCount > 10)                                                   //2S
  {
    count = 0;
    LED_BLUE_L();
  }
}

void LOCK_Operation(uint8_t Op)
{
    switch(Op)
    {
      case 0:                                                                     //关闭
      {
        GPIO_ResetBits(LOCK_GPIO,LOCK_Send1_Pin);
        GPIO_SetBits(LOCK_GPIO,LOCK_Send2_Pin);     
        break;
      }
      case 1:                                                                     //开启
      {
        GPIO_SetBits(LOCK_GPIO,LOCK_Send1_Pin);
        GPIO_ResetBits(LOCK_GPIO,LOCK_Send2_Pin);
        break;
      }
      default:
        break;
    } 
    NeedChangeLockState = 1;
    LockChangeTim = 0;
}