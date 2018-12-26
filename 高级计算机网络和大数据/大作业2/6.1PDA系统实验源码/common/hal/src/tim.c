#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include <stdio.h>
void time_init(void)
  {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
      //打开TIM2外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
                          
   //打开TIM3外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
   //打开TIM4外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

   //**************************************************************************
   //     定时器2设置： 720分频，500ms中断一次，向上计数
   //**************************************************************************
    TIM_TimeBaseStructure.TIM_Period = 50000;
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM2, ENABLE); //使能定时器

   //**************************************************************************
   //     定时器3设置： 720分频，250ms中断一次，向上计数
   //**************************************************************************    
    TIM_TimeBaseStructure.TIM_Period = 25000;
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM3, ENABLE); //使能定时器
    
   //**************************************************************************
   //     定时器4设置： 1440分频，1000ms中断一次，向上计数
   //**************************************************************************
    
    TIM_TimeBaseStructure.TIM_Period = 50000;
    TIM_TimeBaseStructure.TIM_Prescaler = 1439;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //开定时器中断
    
    TIM_Cmd(TIM4, ENABLE);    //使能定时器
    
     // 使能TIM2中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
  
    // 使能TIM3中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    // 使能TIM4中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{ 
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    led1_toggle();
  }    
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
    {
      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
      led2_toggle();
    }  
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{ 
 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
   {
     TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
     printf("This is timer test\n");
   }  
}
