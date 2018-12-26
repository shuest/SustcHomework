
#ifndef USART_H
#define USART_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "NBIOT.h"
#include "stdio.h"

///*U2*/
//extern uint8_t USART2_RxBuf[255];
//extern uint16_t USART2_RxCount;
//extern uint16_t USART2_ANum;
//extern uint8_t USART2_UartReceiving;
//extern uint8_t USART2_UartDataFinishFlag;
//extern uint8_t USART2_TimCount;
///*U2*/

void GPS_uart5_init(void);
extern char receive_buf[MAX_len];
extern char  USART3_receive_buf[MAX_len];
extern char Modbus_buf[MAX_len];
void NVIC_Configuration(void);
void uart3_init(void);
void USART3_Send(uint8_t ch);
void NB_uart3_init(void);
void Initial_UART1(u32 baudrate);
void USART1_ReceiveData(void);
void UART1_Put(unsigned char *Str,unsigned char len);
void LORA_ReceiveData(void);
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num);
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void USART2_ReceiveData(void);
void Usart_SendNumString9600(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num);
void GPS_ReceiveData(void);

#endif