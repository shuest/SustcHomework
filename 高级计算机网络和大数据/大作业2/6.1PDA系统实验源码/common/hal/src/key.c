
#include "key.h"
#include "misc.h"
#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "stdio.h"
/***********************************************************
* ����: exti_init()
* ����: ��ʼ������K1�Ͱ���K2�������ð����ⲿ�ж�
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void exti_init(void)
{  
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* ��ʼ�� GPIOA ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  /* ���� PA.0 �� PA.1 ��Ϊ�������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ��ʼ��AFIOʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* ���ⲿ�ж�0���ӵ� PA.0 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  /* �����ⲿ�ж�0���� */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* �����ⲿ�ж�0Ϊ������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ⲿ�ж�1���� */
  /* �����ⲿ�ж�1Ϊ������ȼ� */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;

  NVIC_Init(&NVIC_InitStructure);
}
