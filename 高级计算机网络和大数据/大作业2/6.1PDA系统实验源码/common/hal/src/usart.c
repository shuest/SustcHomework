
#include <stdio.h>
#include "usart.h"

//注意IAR要加上 _DLIB_FILE_DESCRIPTOR 宏定义
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

extern int RX_Flag;

/***********************************************************
* 名称: uart_init()
* 功能: 串口1初始化程序，将串口初始化为
        - 波特率 = 115200
        - 字长 = 8 位
        - 无停止位
        - 无奇偶校验位
        - 禁止硬件流控制
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void uart2_init()
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* 配置串口2 Tx（PA2）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 Rx（PA3）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口2 的各种参数 */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &USART_InitStructure);
  /* 串口2开始工作 */
  USART_Cmd(USART2, ENABLE);
}

/***********************************************************
* 名称: PUTCHAR_PROTOTYPE
* 功能: 输出一个字节到串口
* 参数: 无
* 返回: 无
* 修改:
* 注释: 这个函数在iar中实际上是putchar函数(文件开头定义)，实现了putchar这个函数
后，编译器就能够使用printf函数。
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART2, (uint8_t) ch);
  
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {}
  
  return ch;
}

/***********************************************************
* 名称: fgetc
* 功能: 从串口接收一个字节
* 参数: 无
* 返回: 无
* 修改:
* 注释: 只要实现了这个函数就可以使用scanf了。
***********************************************************/
int fgetc(FILE *f)
{
    
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {}
    RX_Flag = 1;
    return (int)USART_ReceiveData(USART2);
}