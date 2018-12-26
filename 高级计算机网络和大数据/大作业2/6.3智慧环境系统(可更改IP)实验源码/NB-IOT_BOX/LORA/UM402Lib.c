#include "UM402Lib.h"
#include "UM402.h"
#include "gpio.h"
#include "UART4.h"
#include "lock.h"

extern uint8_t USART4_TimCount;
extern uint8_t USART4_UartReceiving;
extern uint16_t USART4_RxCount;
extern uint16_t USART4_ANum;
extern uint8_t USART4_UartDataFinishFlag;
extern uint8_t NeedCloseLedFlag;
extern void  Data_Um402(void);
extern uint8_t NeedChangeLockState;
extern uint8_t LockChangeTim;
extern void LCD12864_wstr(unsigned char y_add , unsigned char x_add , unsigned char *str);
extern uint8_t NeedFrashScreeNetFlag;
extern uint8_t CloseLedORScreeTim;
extern uint8_t NeedFrashScreeLoraFlag;

void LORA_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE); 
  
    /*UM402 LORA GPIO  5 SETB  7 SETA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  LORA_SETA_H();
  LORA_SETB_H();
  
  /*AUX*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB , &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void LORA_DeviceInit(void)
{
  LORA_GPIOConfig();
  Initial_UART4(9600);
  LORA_Pwor_H();                                                                //使能LORA
}

void LORAReData(void)
{
  if(USART4_TimCount >= 2 && USART4_UartReceiving)                                     //10ms
  {
    USART4_TimCount = 0;
    if(USART4_ANum == USART4_RxCount)                                                 //数据接收完成
    {
      USART4_UartDataFinishFlag = 1;
      USART4_UartReceiving = 0;                                                    //串口接受数据结束
      LED_BLUE_H();                                                             //收到LORA数据
      NeedCloseLedFlag = 1;
      CloseLedORScreeTim = 0;
    }
    else
    {
      USART4_ANum = USART4_RxCount;
    }
  }
}

void FrashOption(void)
{
  if(CloseLedORScreeTim >= 10)                                                  //2s  清屏
  {
    if(NeedCloseLedFlag)
    {   
      NeedCloseLedFlag = 0;
      CloseLedORScreeTim = 0;
      LED_BLUE_L();
      LED_RED_L();
      BEEP_L();
    }
    if(NeedFrashScreeLoraFlag)
    {
      NeedFrashScreeLoraFlag = 0;
      CloseLedORScreeTim = 0;
      LCD12864_wstr(2,0,"                ");
    }
    if(NeedFrashScreeNetFlag)
    {
      NeedFrashScreeNetFlag = 0;
      CloseLedORScreeTim = 0;
      LCD12864_wstr(3,0,"                ");
      LCD12864_wstr(2,0,"                ");
    }
  }
  
  if(LockChangeTim >= 80)                                                        //400ms
  {
    LockChangeTim = 0;
    if(NeedChangeLockState)
    {
      NeedChangeLockState = 0;
      GPIO_ResetBits(LOCK_GPIO,LOCK_Send1_Pin);
      GPIO_ResetBits(LOCK_GPIO,LOCK_Send2_Pin);
    }
  }
}

void LORA_ReceiveData(void)
{
  LORAReData();                                                                 //LORA接收数据
  
  if(USART4_UartDataFinishFlag)                                                 //LORA数据处理              具体可看"RF空中协议"
    Data_Um402();
  
  FrashOption();                                                                //刷新屏幕
    
}