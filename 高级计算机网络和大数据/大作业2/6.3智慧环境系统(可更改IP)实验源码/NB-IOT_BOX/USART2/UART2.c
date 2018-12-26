#include "UART2.h"
#include <stdio.h>
#include "gpio.h"
#include "usart.h"
   
//extern uint8_t USART2_TimCount;

//void USART2_IRQHandler(void)  /*USART2中断函数*/       
//{
//  uint8_t USART2_RxData = 0;
//  
//  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)   //接收中断 (接收寄存器非空) 
//  {   
//      USART2_UartReceiving = 1; 
//      
//      USART2_RxData = USART2->DR;
//      
//      if (USART2_RxCount < 255)
//      {
//        USART2_RxBuf[USART2_RxCount++] = USART2_RxData;
//      }
//      else 
//      {
//        USART2_RxCount = 0;
//      }
//  } 
//}


//void USART2_ReceiveData(void)   /*判断是都接收完成函数*/
//{
//  if(USART2_TimCount >= 2 && USART2_UartReceiving)                                     //10ms
//  {  
//    USART2_TimCount = 0;
//    if(USART2_ANum == USART2_RxCount)                                                 //数据接收完成
//    {
//      USART2_UartDataFinishFlag=1;
//      USART2_UartReceiving = 0;                                                    //串口接受数据结束
//    }
//    else
//    {
//      USART2_ANum = USART2_RxCount;
//    }
//    
//  }
//}


/**************************实现函数********************************************
*函数原型:		void UART2_Put_Char(unsigned char DataToSend)
*功　　能:		RS232发送一个字节
输入参数：
		unsigned char DataToSend   要发送的字节数据
输出参数：没有	
*******************************************************************************/
void UART2_Put_Char(unsigned char DataToSend)
{
	//将要发送的字节写到UART2的发送缓冲区
  USART_SendData(USART2, (unsigned char) DataToSend);
	//等待发送完成
  while (!(USART2->SR & USART_FLAG_TXE));

//	U2TxBuffer[U2count++] = DataToSend;  
//   USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}


void UART2_Put(unsigned char *Str,unsigned char len)
{
  unsigned char i;
  for(i=0;i<len;i++)
  {
     UART2_Put_Char(*Str);
     Str++;
  }
}
  

/**************************实现函数********************************************
*函数原型:		void UART2_Put_String(unsigned char *Str)
*功　　能:		RS232发送字符串
输入参数：
		unsigned char *Str   要发送的字符串
输出参数：没有	
*******************************************************************************/
void UART2_Put_String(unsigned char *Str)
{
    //判断Str指向的数据是否有效.
    while(*Str){
    //是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
    if(*Str=='\r')UART2_Put_Char(0x0d);
            else if(*Str=='\n')UART2_Put_Char(0x0a);
                    else UART2_Put_Char(*Str);
    //等待发送完成.
    //while (!(USART1->SR & USART_FLAG_TXE));
    //指针++ 指向下一个字节.
    Str++;
    }
}

