#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "alarm.h"

#define GREEN_1() GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define GREEN_0() GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define RED_1()   GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define RED_0()   GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define BUZZ_1()  GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define BUZZ_0()  GPIO_ResetBits(GPIOB, GPIO_Pin_0)


static volatile int flag;
void alarm_set(int f) 
{
  flag = f;
}

 void onoff_set(unsigned char onoff,char c) 
{
if(onoff)
 {
    switch(c)
    { case '8': flag = 8;break;
      case '1':RED_1();flag |=1;break;
      case '2':GREEN_1();flag |=2;break;
      case '3':GREEN_1();RED_1(); flag |=3;break;
      case '4':BUZZ_1(); flag |=4;break;
      case '5':RED_1();BUZZ_1();flag |=5;break;
      case '6':GREEN_1();BUZZ_1();flag |=6;break;
      case '7':GREEN_1();RED_1();BUZZ_1();flag |=7;break;
     
    default:break;
    }
 }
else
{ switch(c)
    { case '8': flag =0 ;GREEN_0(); RED_0(); BUZZ_0(); break;
      case '1':RED_0();flag &=~(8+1);break;
      case '2':GREEN_0();flag &=~(8+2);break;
      case '3':GREEN_0();RED_0(); flag &=~(8+3);break;
      case '4':BUZZ_0(); flag &=~(8+4);break;
      case '5':RED_0();BUZZ_0();flag &=~(8+5);break;
      case '6':GREEN_0();BUZZ_0();flag &=~(8+6);break;
      case '7':GREEN_0();RED_0();BUZZ_0();flag &=~(8+7);break;
    default:break;
    }
}

}

void alarm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //配置GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG  使PB3 PB4 PA15为IO口*/
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GREEN_0();
    RED_0();
    BUZZ_0(); 
        
      //打开TIM3外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
   //**************************************************************************
   //     定时器3设置： 1440分频，1000ms中断一次，向上计数
   //**************************************************************************
    TIM_TimeBaseStructure.TIM_Period = 49999;
    TIM_TimeBaseStructure.TIM_Prescaler = 1439;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //初始化定时器
    
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //开定时器中断
    
    // 使能TIM3中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);
   
    TIM_Cmd(TIM3, ENABLE); //使能定时器
    
    
}

void TIM3_IRQHandler(void)
{
  static int flag_on=-1;
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    // if(flag == 1) {    
      if(flag == 8) {  
      flag_on++;
      if (flag_on > 1) flag_on = 0;

    switch(flag_on)
    {
      case 0:
         RED_0();
         GREEN_1();        
         BUZZ_0();
        break;
      case 1:
         RED_1();
         GREEN_0();               
         BUZZ_1();
        break;
    default:break;
    }   
     } 
  }  
}