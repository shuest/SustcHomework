#include "wdg.h"
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"
#include <stdio.h>

void iwdg_init(void)
{
  // ʹ��д IWDG_PR �� IWDG_RLR �Ĵ��� 
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  // IWDG ����ʱ��: 40KHz(LSI) / 32 = 1.25 KHz 
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  //�趨����ֵ0x4dc ����Լ1ms��Ҫ����һ��
  IWDG_SetReload(0x4DC);

  // ����IWDG 
  IWDG_ReloadCounter();

  // ʹ�� IWDG (LSI ʱ���Զ���Ӳ��ʹ��)
  IWDG_Enable();
}

void wwdg_init(void)
{
    //ʹ�� WWDG ʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  // WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  //�趨����ֵΪ 65 
  WWDG_SetWindowValue(65);

  //ʹ�� WWDG ���趨����ֵΪ127, WWDGʱ�� = ~4 ms * 64 = 262 ms */
  WWDG_Enable(127);

  //�� EWI ��־
  WWDG_ClearFlag();

  //ʹ��  EW �ж�
  WWDG_EnableIT();
  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void WWDG_IRQHandler(void)
{
  //���� WWDG ������ֵ
  WWDG_SetCounter(0x7F);
  //�� EWI ��־
  WWDG_ClearFlag();
  led1_toggle();
  printf("wwdg reloaded!\n");
}