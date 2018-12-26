#ifndef __UART2_H
#define __UART2_H

#include <stdio.h>
#include "stm32f10x.h"


extern unsigned char UART2_pRcv,UART2_RcvOK,UART2_Rcvlend;	//����ָ�롢������ɱ�� 

extern unsigned char UART2_Rcvbuf[100];

// extern unsigned char Uart2_Get_Data;	//����2���յ�����

void Initial_UART2(u32 baudrate);
void UART2_Put_Char(unsigned char DataToSend);
u8 UART2_Get_Char(void);
void UART2_Put_String(unsigned char *Str);


#endif

