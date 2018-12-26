#ifndef __UARTS_H
#define __UARTS_H

#include <stdio.h>
#include "stm32f10x.h"


extern unsigned char UART1_pRcv,UART1_RcvOK;	//接收指针、接收完成标记 

extern unsigned char UART1_Rcvbuf[255];


void 	Initial_UART1(u32 baudrate);
void	NVIC_Configuration(void);
void UART1_Put_Char(unsigned char DataToSend);

 void UART1_Put(unsigned char *Str,unsigned char len);
 
u8 UART1_Get_Char(void);

void UART1_Put_String(unsigned char *Str);

void Uart1_IRQ(void);
void Check_Uart1(void);

#endif
