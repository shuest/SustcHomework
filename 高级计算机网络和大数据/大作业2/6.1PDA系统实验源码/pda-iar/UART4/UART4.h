#ifndef _UART4_H_
#define _UART4_H_
#include "stm32f10x.h"

extern unsigned char UART4_pRcv,UART4_RcvOK;	//接收指针、接收完成标记 

extern unsigned char UART4_Rcvbuf[255];
extern unsigned char LED_BLUE_tick;
extern unsigned char LED_BLUE_Flag;
extern unsigned char LED_RED_Flag;
extern unsigned char LED_RED_tick;
extern unsigned char LED_RED_TIME_OK;

extern unsigned char BEEP_Flag;
extern unsigned char BEEP_tick;
extern unsigned char BEEP_TIME_OK;
void Initial_UART4(u32 baudrate);
void Uart4_IRQ(void);

void UART4_Put_Char(unsigned char DataToSend);
void UART4_Put(unsigned char *Str,unsigned char len);
void UART4_Put_String(unsigned char *Str);
void Check_Uart4(void);
#endif
