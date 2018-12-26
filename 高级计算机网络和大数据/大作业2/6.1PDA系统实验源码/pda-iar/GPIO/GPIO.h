#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f10x.h"

extern void GPIO_Configuration(void);

void Watchdog(void);
void PDA_Init(void);
	/*12864 GPIO 驱动端口*/
#define LCD12864_PW_EN_H()         GPIO_SetBits(GPIOE,GPIO_Pin_7)
#define LCD12864_PW_EN_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_7)

#define LCD12864_BL_EN_H()         GPIO_SetBits(GPIOE,GPIO_Pin_8)
#define LCD12864_BL_EN_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_8)

#define LCD12864_RST_H()           GPIO_SetBits(GPIOE,GPIO_Pin_9)
#define LCD12864_RST_L()           GPIO_ResetBits(GPIOE,GPIO_Pin_9)

#define LCD12864_RS_CS_H()         GPIO_SetBits(GPIOE,GPIO_Pin_10)       //（数据命令）寄存器选择输入 
#define LCD12864_RS_CS_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_10)

#define LCD12864_SDI_H()		      GPIO_SetBits(GPIOE, GPIO_Pin_11)           //液晶串行数据输入端
#define LCD12864_SDI_L() 		      GPIO_ResetBits(GPIOE, GPIO_Pin_11)

#define LCD12864_SCLK_H()         GPIO_SetBits(GPIOE,GPIO_Pin_12)          //液晶时钟输入端
#define LCD12864_SCLK_L()         GPIO_ResetBits(GPIOE,GPIO_Pin_12)


	/*UM402 LORA GPIO 驱动端口*/
#define LORA_SETA_H()         GPIO_SetBits(GPIOB,GPIO_Pin_7)       
#define LORA_SETA_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define LORA_AUX_H()		      GPIO_SetBits(GPIOB, GPIO_Pin_6)           
#define LORA_AUX_L() 		      GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define LORA_SETB_H()         GPIO_SetBits(GPIOB,GPIO_Pin_5)          
#define LORA_SETB_L()         GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LORA_Pwor_H()         GPIO_SetBits(GPIOD,GPIO_Pin_5)          
#define LORA_Pwor_L()         GPIO_ResetBits(GPIOD,GPIO_Pin_5)

  /*bmp085 驱动端口*/	
#define SDA_H         GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define SDA_L         GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)

#define SCL_H         GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define SCL_L         GPIO_ResetBits(GPIOB,GPIO_Pin_13)

	/*按键 GPIO 驱动端口*/	
#define AUTO         	 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define MANUAL         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define LOCK           GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define UNLOCK         GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

#define LOCK_CHECK     GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

#define REMOVE_ALARM   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define LIFT_ROD       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

	/*LED  驱动端口*/	
	
// #define LED_BLUE_H()         GPIO_SetBits(GPIOA,GPIO_Pin_1)       
// #define LED_BLUE_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_1)	
	
#define LED_BLUE_H()         GPIO_SetBits(GPIOA,GPIO_Pin_6)       
#define LED_BLUE_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define LED_RED_H()         GPIO_SetBits(GPIOA,GPIO_Pin_7)       
#define LED_RED_L()         GPIO_ResetBits(GPIOA,GPIO_Pin_7)




  /*蜂鸣器 驱动端口*/	
	
#define BEEP_H()         GPIO_SetBits(GPIOC,GPIO_Pin_4)       
#define BEEP_L()         GPIO_ResetBits(GPIOC,GPIO_Pin_4)

  /*复位引脚驱动端口*/	
	
#define RESET_WDI_H()         GPIO_SetBits(GPIOC,GPIO_Pin_1)       
#define RESET_WDI_L()         GPIO_ResetBits(GPIOC,GPIO_Pin_1)


#endif
