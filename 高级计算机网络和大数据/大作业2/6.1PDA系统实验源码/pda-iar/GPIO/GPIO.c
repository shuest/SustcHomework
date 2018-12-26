#include "GPIO.h"
RCC_ClocksTypeDef RCC_ClockFreq;

extern unsigned  int Self_Test;

/*******************************************************************************
* 函 数 名         : void GPIO_Configuration(void)
* 函数功能		     : GPIO配置
* 输    入         : 
* 输    出         :
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  		  //开启AFIO时钟
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //端口时钟	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //端口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //端口时钟

  /*12864 GPIO 驱动端口*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  Delay_ms(10);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    	//50M时钟速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*bmp085 驱动端口*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; //SDA 12   SCL 13      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	     //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //50M时钟速度
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*按键 GPIO 驱动端口*/	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   AUTO
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   MANUAL
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   LOCK
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   UNLOCK
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   LOCK_CHECK
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   REMOVE_ALARM
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入   LIFT_ROD
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*LED 驱动端口*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*蜂鸣器 驱动端口*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*复位引脚*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOC,GPIO_Pin_1);
}

/*******************************************************************************
* 函 数 名         : void PDA_Init(void)
* 函数功能		     : PDA初始化
* 输    入         : 
* 输    出         :
*******************************************************************************/
void PDA_Init(void)
{
  SystemInit();                       //配置系统时钟为72M 使用外部8M晶体+PLL
  SysTick_Init();
  RCC_GetClocksFreq(&RCC_ClockFreq);	//初始化时钟源
  GPIO_Configuration();               //IO口初始化、LED初始化、按键初始化
  I2C_Init1();
  AT24CXX_Init();											//EEprom初始化
  Self_Test=AT24CXX_ReadLenByte(Reset_Flag_Addr,2);
  LCD12864_Init();                    //LCD12864初始化	
  Delay_ms(10);                       //延时	
  LORA_Pwor_H();
  LCD1264_clear();                    //LCD清屏	
  Delay_ms(10);                       //延时
  LORA_SETA_H();
  LORA_SETB_H();
  Adc_Init(); 									      //AD初始化
  Initial_UART1(57600);  
  Initial_UART4(9600);

  SHTXX_Init();      								  //温湿度初始化
  bmp280Init(); 											//数字气压初始化
                                        
  LCD12864_wstr(2,2,"泛太科技");
  LCD12864_wstr(3,1,"PDA  SYSTEM");
  Watchdog();
  Delay_ms(300);
  Watchdog();
  Delay_ms(300);
  Watchdog();
  Delay_ms(300);
  Watchdog();
        
  TIM2_NVIC_Configuration();  	 //TIM2 定时配置 
  TIM2_Configuration();

  TIM3_NVIC_Configuration();  	 //TIM3 定时配置 
  TIM3_Configuration();
        
  TIM4_NVIC_Configuration();     //TIM4 定时配置 
  TIM4_Configuration();
  Watchdog();
}

/*******************************************************************************
* 函 数 名         : void Watchdog(void)
* 函数功能		     : 看门狗喂狗
* 输    入         : 
* 输    出         :
*******************************************************************************/
void Watchdog(void)
{
  RESET_WDI_H();
  Delay_us(10);
  RESET_WDI_L();
  Delay_us(10);
}

void WatchDogInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOC,GPIO_Pin_1);
}

