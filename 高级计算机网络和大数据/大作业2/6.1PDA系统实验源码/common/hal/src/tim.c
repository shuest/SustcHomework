#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include <stdio.h>
void time_init(void)
  {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
      //��TIM2����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
                          
   //��TIM3����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
   //��TIM4����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

   //**************************************************************************
   //     ��ʱ��2���ã� 720��Ƶ��500ms�ж�һ�Σ����ϼ���
   //**************************************************************************
    TIM_TimeBaseStructure.TIM_Period = 50000;
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ����ʱ��
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //����ʱ���ж�
    
    TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��

   //**************************************************************************
   //     ��ʱ��3���ã� 720��Ƶ��250ms�ж�һ�Σ����ϼ���
   //**************************************************************************    
    TIM_TimeBaseStructure.TIM_Period = 25000;
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //��ʼ����ʱ��
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //����ʱ���ж�
    
    TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��
    
   //**************************************************************************
   //     ��ʱ��4���ã� 1440��Ƶ��1000ms�ж�һ�Σ����ϼ���
   //**************************************************************************
    
    TIM_TimeBaseStructure.TIM_Period = 50000;
    TIM_TimeBaseStructure.TIM_Prescaler = 1439;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //��ʼ����ʱ��
    
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //����ʱ���ж�
    
    TIM_Cmd(TIM4, ENABLE);    //ʹ�ܶ�ʱ��
    
     // ʹ��TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
  
    // ʹ��TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    // ʹ��TIM4�ж�
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
