
#include <stdio.h>
#include "usart.h"

//ע��IARҪ���� _DLIB_FILE_DESCRIPTOR �궨��
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern int RX_Flag;

/***********************************************************
* ����: uart_init()
* ����: ����1��ʼ�����򣬽����ڳ�ʼ��Ϊ
        - ������ = 115200
        - �ֳ� = 8 λ
        - ��ֹͣλ
        - ����żУ��λ
        - ��ֹӲ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void uart2_init()
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* ���ô���2 Tx��PA2��Ϊ���츴��ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 Rx��PA3��Ϊ��������ģʽ */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ���ô���2 �ĸ��ֲ��� */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  /* ����2��ʼ���� */
  USART_Cmd(USART2, ENABLE);
}

/***********************************************************
* ����: PUTCHAR_PROTOTYPE
* ����: ���һ���ֽڵ�����
* ����: ��
* ����: ��
* �޸�:
* ע��: ���������iar��ʵ������putchar����(�ļ���ͷ����)��ʵ����putchar�������
�󣬱��������ܹ�ʹ��printf������
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART2, (uint8_t) ch);
  
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {}
  
  return ch;
}

/***********************************************************
* ����: fgetc
* ����: �Ӵ��ڽ���һ���ֽ�
* ����: ��
* ����: ��
* �޸�:
* ע��: ֻҪʵ������������Ϳ���ʹ��scanf�ˡ�
***********************************************************/
int fgetc(FILE *f)
{
    
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {}
    RX_Flag = 1;
    return (int)USART_ReceiveData(USART2);
}