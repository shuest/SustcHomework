#include <stdio.h>
#include "usart.h"
#include "NBIOT.h"
#include "string.h"
#include "gps.h"
#include "Command.h"
#include "gpio.h"
#include "SHT10.h"
#include "BMP280.h"
#include "adc.h"
#include "delay.h"
#include "lock.h"

//注意IAR要加上 _DLIB_FILE_DESCRIPTOR 宏定义
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int RX_Flag = 0;                        //串口接收到数据

/*NB-IOT*/
uint8_t NB_RxFlag = 0;                  //NB_串口接受标志位
uint8_t NB_SignCount = 0;               //符号计数
uint8_t NB_CMDErrorFlag = 0;            //返回错误标志
uint8_t NB_ReceMSGFlag = 0;             //接收到数据标志
uint8_t NB_ReceOKFlag = 0;              //接收到OK

u8 UART1_Rcvbuf[255]={00};

uint16_t GPS_ANum = 0xff;

extern uint8_t CloseLedORScreeTim;
extern uint8_t NeedCloseLedFlag;
extern uint8_t NeedFrashScreeNetFlag;
extern uint8_t Get_NBReceiveDataLength(uint8_t *string);

extern uint8_t USART3_TimCount;
extern int Rec_length;
extern uint8_t NSORF_ComFlag;
extern uint8_t GPS_UartTim;
extern uint8_t GPS_UartDataFinishFlag;
/*NB-IOT*/

/*GPS*/
uint8_t GPS_RxBuf[GPS_MAX_len] = {0};
uint16_t GPS_RxCount = 0;
uint8_t GPS_UartReceiving = 0;
/*GPS*/

/*U1*/
uint8_t USART1_RxBuf[255] = {0};
uint16_t USART1_RxCount = 0;
uint16_t USART1_ANum = 0xff;
uint8_t USART1_UartReceiving = 0;
uint8_t USART1_UartDataFinishFlag = 0;
extern uint8_t USART1_TimCount;
/*U1*/

/*U4*/
uint8_t USART4_RxBuf[255] = {0};
uint16_t USART4_RxCount = 0;
uint16_t USART4_ANum = 0xff;
uint8_t USART4_UartReceiving = 0;
uint8_t USART4_UartDataFinishFlag = 0;
/*U4*/

/*U2*/
uint8_t USART2_RxBuf[255] = {0};
uint16_t USART2_RxCount = 0;
uint16_t USART2_ANum = 0xff;
uint8_t USART2_UartReceiving = 0;
uint8_t USART2_UartDataFinishFlag = 0;
/*U2*/

/*U3*/
uint8_t NB_RxBuf[255] = {0};
uint16_t NB_RxCount = 0;
uint16_t USART3_ANum = 0xff;
uint8_t USART3_UartReceiving = 0;
uint8_t USART3_UartDataFinishFlag = 0;
/*U3*/

extern uint16_t LoraSensorCycleCount;
extern uint16_t Sensor_UploadCycle;
extern void Float2Hex(uint8_t *buf,float num);
extern void Float2Hex2(uint8_t *buf,float num);
//温度转换结果
extern float TempResult;
//湿度转换结果
extern float HumResult;

//气压
extern float BMP280_Pressure;
//海拔
extern float BMP280_High;
//温度
extern float BMP280_Tem;


void UART1_Put_Char(unsigned char DataToSend)
{
  USART_SendData(USART1, (unsigned char) DataToSend);
  while (!(USART1->SR & USART_FLAG_TXE)); 
}

 void UART1_Put(unsigned char *Str,unsigned char len)
{
  unsigned char i;
  for(i=0;i<len;i++)
  {
    UART1_Put_Char(*Str);
     Str++;
  }
}

/*****************发送一个字符**********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
  USART_SendData(pUSARTx,ch);
          
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************发送字符串**********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
  unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
      k++;
  } while(*(str + k)!='\0');

  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/********************发送固定的长度*******************************/
void Usart_SendNumString(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)            //57600
{
  unsigned int k=0;
  for(k = 0;k < Num;k++)
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(200);
  }
  
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/********************发送固定的长度*******************************/
void Usart_SendNumString9600(USART_TypeDef * pUSARTx, uint8_t *str, uint8_t Num)            //9600
{
  unsigned int k=0;
  for(k = 0;k < Num;k++)
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      delay_us(1000);
  }
  
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;
  
  if(NBIOT)
  {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //NB
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStruct);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //GPRS
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStruct);
  }
  
  if(GPRS)
  {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //GPRS
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStruct);
  }
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //LORA
  NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //GPS
  NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);                               //基本串口
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  NVIC_Init(&NVIC_InitStruct);
  
}

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
void Initial_UART1(u32 baudrate)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

  /* 配置串口1 Tx（PA9）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口1 Rx（PA10）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* 配置串口1 的各种参数 */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  /* 串口1开始工作 */
  USART_Cmd(USART1, ENABLE);
  
  USART_ClearFlag(USART1,USART_FLAG_TC);
}

void NB_uart3_init(void)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* 配置串口3 Tx（PB10）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* 配置串口3 Rx（PB11）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* 配置串口3 的各种参数 */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  /* 串口3开始工作 */
  USART_Cmd(USART3, ENABLE);
  
  USART_ClearFlag(USART3,USART_FLAG_TC);
}

void GPS_uart5_init(void)
{  
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  /* 配置串口5 Tx（PC12）为推挽复用模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* 配置串口5 Rx（PD2）为浮空输入模式 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* 配置串口5 的各种参数 */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(UART5, &USART_InitStructure);
  USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
  /* 串口5开始工作 */
  USART_Cmd(UART5, ENABLE);
  USART_ClearFlag(UART5,USART_FLAG_TC);
}


/*判断是都接收完成函数*/
void USART1_ReceiveData(void)
{
  if(USART1_TimCount >= 2 && USART1_UartReceiving)                                     //10ms
  {
    USART1_TimCount = 0;
    if(USART1_ANum == USART1_RxCount)                                                 //数据接收完成
    {
      USART1_UartDataFinishFlag = 1;
      USART1_UartReceiving = 0;                                                    //串口接受数据结束
    }
    else
    {
      USART1_ANum = USART1_RxCount;
    }
    
  }
  
  if(USART1_UartDataFinishFlag)
  {
    Data_UART();                                                                //串口1数据获取
    Data_Dispose();                                                             //根据串口数值设置参数
  }
  
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
  USART_SendData(USART3, (uint8_t) ch);
  Usart_SendByte(USART1,ch);
  delay_us(200);
  
  while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE))
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
    
    while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
    {}
    return (int)USART_ReceiveData(USART1);
}

/*USART1中断函数*/          
void USART1_IRQHandler(void)
{
  uint8_t USART1_RxData = 0;
  if(USART_GetFlagStatus(USART1, USART_IT_RXNE))
  {   
    USART1_UartReceiving = 1;
    USART1_RxData = USART_ReceiveData(USART1); 
    if (USART1_RxCount < sizeof(USART1_RxBuf)-5)
    {
      USART1_RxBuf[USART1_RxCount++] = USART1_RxData;
    }
    else
    {
      USART1_RxCount = 0;
    }
  }
}

extern uint8_t USART2_TimCount;


/*USART2 GPRS中断函数*/ 
void USART2_IRQHandler(void)  /*USART2中断函数*/       
{
  uint8_t USART2_RxData = 0;
  
  if(USART_GetFlagStatus(USART2,USART_IT_RXNE))   //接收中断 (接收寄存器非空) 
  {   
      USART2_UartReceiving = 1; 
      
      USART2_RxData = USART_ReceiveData(USART2);
      Usart_SendByte(USART1,USART2_RxData);
      
      if (USART2_RxCount < sizeof(USART2_RxBuf)-5)
      {
        USART2_RxBuf[USART2_RxCount++] = USART2_RxData;
      }
      else 
      {
        USART2_RxCount = 0;
      }
  } 
}

void USART2_ReceiveData(void)   /*判断是都接收完成函数*/
{
  if(USART2_TimCount >= 2 && USART2_UartReceiving)                                     //10ms
  {  
    USART2_TimCount = 0;
    if(USART2_ANum == USART2_RxCount)                                                 //数据接收完成
    {
      USART2_UartDataFinishFlag = 1;
      USART2_UartReceiving = 0;                                                    //串口接受数据结束
    }
    else
    {
      USART2_ANum = USART2_RxCount;
    }
    
  }
}


/*USART3中断函数  NB*/
void USART3_IRQHandler(void)
{
  uint8_t NB_RxData = 0;
  if(USART_GetFlagStatus(USART3, USART_IT_RXNE))
  {   
    USART3_UartReceiving = 1;
    NB_RxData = USART_ReceiveData(USART3); 
    Usart_SendByte(USART1,NB_RxData);
    if (NB_RxCount < sizeof(NB_RxBuf)-5)
    {
      NB_RxBuf[NB_RxCount++] = NB_RxData;
    }
    else
    {
      NB_RxCount = 0;
    }
  }
}

void USART3_ReceiveData(void)   /*判断是都接收完成函数*/
{
  if(USART3_TimCount >= 4 && USART3_UartReceiving)                                     //20ms
  {  
    USART3_TimCount = 0;
    if(USART3_ANum == NB_RxCount)                                                 //数据接收完成
    {
      USART3_UartDataFinishFlag = 1;
      USART3_UartReceiving = 0;                                                    //串口接受数据结束
    }
    else
    {
      USART3_ANum = NB_RxCount;
    }
    
  }
}

/*USART4中断函数 LORA*/          
void UART4_IRQHandler(void)
{
  uint8_t USART4_RxData = 0;
  if(USART_GetFlagStatus(UART4, USART_IT_RXNE))
  {   
    USART4_UartReceiving = 1;
    USART4_RxData = USART_ReceiveData(UART4); 
    if (USART4_RxCount < sizeof(USART4_RxBuf)-5)
    {
      USART4_RxBuf[USART4_RxCount++] = USART4_RxData;
    }
    else
    {
      USART4_RxCount = 0;
    }
  }
}

/*USART5中断函数    GPS*/          
void UART5_IRQHandler(void)
{
  uint8_t GPS_RxData = 0;
  if(USART_GetFlagStatus(UART5, USART_IT_RXNE))
  {   
    GPS_UartReceiving = 1;
    GPS_RxData = USART_ReceiveData(UART5); 
  //  Usart_SendByte(USART1,GPS_RxData);
    if (GPS_RxCount < sizeof(GPS_RxBuf)-5)
    {
      GPS_RxBuf[GPS_RxCount++] = GPS_RxData;
    }
    else
    {
      GPS_RxCount = 0;
    }
  }
}

void GPS_ReceiveData(void)
{
  if(GPS_UartTim >= 1 && GPS_UartReceiving)                                     //5ms
  {
    GPS_UartTim = 0;
    if(GPS_ANum == GPS_RxCount)                                                 //数据接收完成
    {
      GPS_UartDataFinishFlag = 1;
      GPS_UartReceiving = 0;                                                    //串口接受数据结束
    }
    else
    {
      GPS_ANum = GPS_RxCount;
    }
  }
}

