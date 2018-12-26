
#include <stdio.h>
#include "clock.h"

uint32_t ulTimeDelay; //ȫ����ʱ������

/***********************************************************
* ����: clock_init()
* ����: ʱ�ӳ�ʼ������
* ����: ���� ulTick��ÿ�����SystemCoreClock / ulTick ���ж�
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void clock_init(uint32_t ulTick)
{
    if (SysTick_Config(SystemCoreClock / ulTick))
    {
        while(1);
    }
}

/***********************************************************
* ����: SysTick_Handler()
* ����: systick�жϴ�����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void SysTick_Handler(void)
{
  (void)SysTick->CTRL;
  SCB->ICSR = SCB_ICSR_PENDSTCLR;
  ulTimeDelay++;
}