#ifndef _UART4_H_
#define _UART4_H_
#include "stm32f10x.h"
#include "stm32f10x_usart.h"

extern unsigned char UART4_pRcv,UART4_RcvOK;	//接收指针、接收完成标记 

extern unsigned char UART4_Rcvbuf[255];


void Initial_UART4(u32 baudrate);
void Uart4_IRQ(void);

void UART4_Put_Char(unsigned char DataToSend);
void UART4_Put(unsigned char *Str,unsigned char len);
void UART4_Put_String(unsigned char *Str);
#endif
