#ifndef __UART2_H
#define __UART2_H

#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_usart.h"



void USART2_ReceiveData(void);
void Initial_UART2(u32 baudrate);
void UART2_Put_Char(unsigned char DataToSend);
u8 UART2_Get_Char(void);
void UART2_Put_String(unsigned char *Str);

void UART2_Put(unsigned char *Str,unsigned char len);

#endif

