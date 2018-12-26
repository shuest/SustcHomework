
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define D4 1 
#define D5 2

//Ϊ�˿���ֲ�ԣ� ��LED�����ţ��˿ڣ�ʼ�ն�������
#define D4_GPIO_PIN GPIO_Pin_8
#define D5_GPIO_PIN GPIO_Pin_9
#define D4_GPIO_PORT GPIOB
#define D5_GPIO_PORT GPIOB
#define D4_GPIO_ACC RCC_APB2Periph_GPIOB
#define D5_GPIO_ACC RCC_APB2Periph_GPIOB

/***********************************************************
* ����: leds_init()
* ����: ��ʼ��led��GPIOʱ�ӣ�����ʼ��Ϊ���ģʽ
* ����: ��
* ����: ��
* �޸�:
* ע��: 1) ��ʹ��stm32��ledǰ�������ȵ��������ʼ������
***********************************************************/

void leds_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ��ʼ��GPIOʱ�� */
  RCC_APB2PeriphClockCmd(D4_GPIO_ACC | D5_GPIO_ACC, ENABLE);

  /* ����LED������Ϊ������� */
  GPIO_InitStructure.GPIO_Pin = D4_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = D5_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D5_GPIO_PORT, &GPIO_InitStructure);
  
  //D4��D5Ĭ�Ϲر�
  D4_off();
  D5_off();
}

/***********************************************************
* ����: D4_on()
* ����: ����D4
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D4_off(void)
{
  GPIO_SetBits(D4_GPIO_PORT, D4_GPIO_PIN);
}

/***********************************************************
* ����: D5_on()
* ����: ����D5
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D5_off(void)
{
  GPIO_SetBits(D5_GPIO_PORT, D5_GPIO_PIN);
}

/***********************************************************
* ����: D4_off()
* ����: Ϩ��D4
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D4_on(void)
{
  GPIO_ResetBits(D4_GPIO_PORT, D4_GPIO_PIN);
}

/***********************************************************
* ����: D5_off()
* ����: Ϩ��D5
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D5_on(void)
{
  GPIO_ResetBits(D5_GPIO_PORT, D5_GPIO_PIN);
}

/***********************************************************
* ����: D4_toggle()
* ����: ��תD4
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D4_toggle(void)
{
  GPIO_WriteBit(D4_GPIO_PORT, D4_GPIO_PIN, !GPIO_ReadOutputDataBit(D4_GPIO_PORT, D4_GPIO_PIN));
}

/***********************************************************
* ����: D5_toggle()
* ����: ��תD5
* ����: ��
* ����: ��
* �޸�:
* ע��:
***********************************************************/
void D5_toggle(void)
{
  GPIO_WriteBit(D5_GPIO_PORT, D5_GPIO_PIN, !GPIO_ReadOutputDataBit(D5_GPIO_PORT, D5_GPIO_PIN));
}
