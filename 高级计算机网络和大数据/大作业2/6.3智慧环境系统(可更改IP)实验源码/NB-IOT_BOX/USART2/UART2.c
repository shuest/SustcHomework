#include "UART2.h"
#include <stdio.h>
#include "gpio.h"
#include "usart.h"
   
//extern uint8_t USART2_TimCount;

//void USART2_IRQHandler(void)  /*USART2�жϺ���*/       
//{
//  uint8_t USART2_RxData = 0;
//  
//  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)   //�����ж� (���ռĴ����ǿ�) 
//  {   
//      USART2_UartReceiving = 1; 
//      
//      USART2_RxData = USART2->DR;
//      
//      if (USART2_RxCount < 255)
//      {
//        USART2_RxBuf[USART2_RxCount++] = USART2_RxData;
//      }
//      else 
//      {
//        USART2_RxCount = 0;
//      }
//  } 
//}


//void USART2_ReceiveData(void)   /*�ж��Ƕ�������ɺ���*/
//{
//  if(USART2_TimCount >= 2 && USART2_UartReceiving)                                     //10ms
//  {  
//    USART2_TimCount = 0;
//    if(USART2_ANum == USART2_RxCount)                                                 //���ݽ������
//    {
//      USART2_UartDataFinishFlag=1;
//      USART2_UartReceiving = 0;                                                    //���ڽ������ݽ���
//    }
//    else
//    {
//      USART2_ANum = USART2_RxCount;
//    }
//    
//  }
//}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Put_Char(unsigned char DataToSend)
*��������:		RS232����һ���ֽ�
���������
		unsigned char DataToSend   Ҫ���͵��ֽ�����
���������û��	
*******************************************************************************/
void UART2_Put_Char(unsigned char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART2�ķ��ͻ�����
  USART_SendData(USART2, (unsigned char) DataToSend);
	//�ȴ��������
  while (!(USART2->SR & USART_FLAG_TXE));

//	U2TxBuffer[U2count++] = DataToSend;  
//   USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  
}


void UART2_Put(unsigned char *Str,unsigned char len)
{
  unsigned char i;
  for(i=0;i<len;i++)
  {
     UART2_Put_Char(*Str);
     Str++;
  }
}
  

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART2_Put_String(unsigned char *Str)
*��������:		RS232�����ַ���
���������
		unsigned char *Str   Ҫ���͵��ַ���
���������û��	
*******************************************************************************/
void UART2_Put_String(unsigned char *Str)
{
    //�ж�Strָ��������Ƿ���Ч.
    while(*Str){
    //�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
    if(*Str=='\r')UART2_Put_Char(0x0d);
            else if(*Str=='\n')UART2_Put_Char(0x0a);
                    else UART2_Put_Char(*Str);
    //�ȴ��������.
    //while (!(USART1->SR & USART_FLAG_TXE));
    //ָ��++ ָ����һ���ֽ�.
    Str++;
    }
}

