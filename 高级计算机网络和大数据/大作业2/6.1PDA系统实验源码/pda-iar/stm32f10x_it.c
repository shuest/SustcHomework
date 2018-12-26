/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exception s handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

extern void TimingDelay_Decrement(void);




/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */

/*=====================================================================================================*/
/*=====================================================================================================*/


/*=====================================================================================================*/
/*=====================================================================================================*/
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  TimingDelay_Decrement();	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
		
			if(Uart1_Recving==1)
			{
				Uart1_Tick++;
				if(Uart1_Tick==2)
				{
					Uart1_Tick = 0;
			  	Uart1_OV=1;
			  	Uart1_Recving=0;
				}
			}
			
			if(Uart4_Recving==1)
			{
				Uart4_Tick++;
				if(Uart4_Tick==2)
				{	 
					Uart4_Tick=0;
					Uart4_OV=1;
					Uart4_Recving=0;
				}
			}
			
		  if(Key_Tick++>20)
			{
				Key_Flag=1;		
			}

// 			if(Lcd_Tick++>4000) 			  //4s LCDË¢ÐÂ
// 			{
// 				Lcd_Tick = 0;
// 				if(Lcd_Ready_Flag == 1)
// 				{
// 					Lcd_Ready_Flag = 0;
// 					Lcd_Flag = 1;	
// 				}

// 			}
			
			if(Lcd_Ready_Flag == 1)			 //Ã¿5ms  Lcd_Tick2++£¬ 2s LCDË¢ÐÂ   
			{
				Lcd_Tick2++;
				if(Lcd_Tick2>400) 			  
				{
					Lcd_Tick2 = 0;
					Lcd_Ready_Flag = 0;
			  	Lcd_Flag = 1;	
				}
			}
					
			if(LED_BLUE_Flag==1)
			{
					LED_BLUE_tick++;
					if(LED_BLUE_tick++>200)
					{
						LED_BLUE_tick=0;
						LED_BLUE_Flag=0;	
					}				
			}
					
			if(LED_RED_Flag==1)
			{
				LED_RED_tick++;
				if(LED_RED_tick++>200)
				{
					LED_RED_tick=0;
					LED_RED_TIME_OK=(0xFF^LED_RED_TIME_OK);
				}
			}
				 
				
			if(BEEP_Flag==1)
				{
					BEEP_tick++;
					if(BEEP_tick++>200)
					{
						BEEP_tick=0;
						BEEP_TIME_OK=(0xFF^BEEP_TIME_OK);
					}
				}
	}		 	
}

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    
  		 time3++;
	}		
}
/**
  * @brief  This function handles TIM4 interrupt request.
  * @param  None
  * @retval : None
  */
void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);    
  		 time4++;
			if(time4>65535)
			{time4=0;}
		   Adc_time++;
			if(Adc_time>65535)
			{Adc_time=0;}
	}		 	
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
