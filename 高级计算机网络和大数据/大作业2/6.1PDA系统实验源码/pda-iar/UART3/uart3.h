#ifndef _UART3_H_
#define _UART3_H_
#include "stm32f10x.h"

extern unsigned char UART3_pRcv,UART3_RcvOK,UART3_Rcvlend;	//接收指针、接收完成标记 

extern unsigned char UART3_Rcvbuf[100];


void Initial_UART3(u32 baudrate);
void Uart3_IRQ(void);

void UART3_Put_Char(unsigned char DataToSend);
void UART3_Put_String(unsigned char *Str);
#endif
