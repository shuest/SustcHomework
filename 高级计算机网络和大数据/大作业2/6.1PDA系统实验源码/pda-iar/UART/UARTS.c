/* 
---------Ӳ���ϵ���������:----------
RS232�ӿڣ�
RS232TXD  -->  PA9  (UART1-TXD)
RS232RXD  -->  PA10 (UART1-RXD)
------------------------------------
 */


#include "UARTS.h"
#include "CmdLib.h"

//ע��IARҪ���� _DLIB_FILE_DESCRIPTOR �궨��
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

unsigned char UART1_pRcv,UART1_RcvOK;	//����ָ�롢������ɱ�� 
unsigned char UART1_Rcvbuf[255]={00};

/*******************************************************************************
* �� �� ��         : void NVIC_Configuration(void)
* ��������		     : NVIC����
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void NVIC_Configuration(void)
{
		/*����һ��NVIC_InitTypeDef ���͵Ľṹ�壬���ֽ�NVIC_InitStructure*/ 
	NVIC_InitTypeDef NVIC_InitStructure; 
	  /**********************************************  
     NVIC_PriorityGroup_0����ռ���ȼ�0λ�����ȼ�4λ
     NVIC_PriorityGroup_1����ռ���ȼ�1λ�����ȼ�3λ
     NVIC_PriorityGroup_2����ռ���ȼ�2λ�����ȼ�2λ
     NVIC_PriorityGroup_3����ռ���ȼ�3λ�����ȼ�1λ
     NVIC_PriorityGroup_4����ռ���ȼ�4λ�����ȼ�0λ
    ***********************************************/ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/*����ѡ�е��ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/*�����˳�ԱNVIC_IRQChannel�е���ռ���ȼ�*/  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/*�����˳�ԱNVIC_IRQChannel�е���Ӧ���ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/*ʹ���ж�����*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*���ÿ⺯������ʼ���ж�����*/
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* �� �� ��         : void Initial_UART1(u32 baudrate)
* ��������		     : ��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
* ��    ��         : u32 baudrate   ����RS232���ڵĲ�����
* ��    ��         :
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�� UART1 ģ���ʱ��  ʹ�� UART1��Ӧ�����Ŷ˿�PA��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //����USART1ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  //�˿ڸ���
// 	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);                              //�˿ڸ���
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);                        //�˿ڸ���

/* ����UART1 �ķ�������
	 ����PA9 Ϊ�������  ˢ��Ƶ��50MHz */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
/* ����UART1 �Ľ�������
	 ����PA10Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);														 
	  
/* 
	UART1������:
	1.������Ϊ���ó���ָ�������� baudrate;
	2. 8λ����			  USART_WordLength_8b;
	3.һ��ֹͣλ			  USART_StopBits_1;
	4. ����żЧ��			  USART_Parity_No ;
	5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
 */
	USART_InitStructure.USART_BaudRate = baudrate;//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���͡�����ʹ��
	
	//����USART1ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	//Ӧ�����õ�UART1
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	//ʹ��USART1�����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(USART1, ENABLE); 
	NVIC_Configuration();
	UART1_pRcv=0x00;
	UART1_RcvOK=0X00;
}

/*******************************************************************************
* �� �� ��         : void USART1_IRQHandler(void)
* ��������		     : �����жϴ�����
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)                //�����ж� (���ռĴ����ǿ�) 
		{
			  
        START_TIME2(); // TIM2 ��ʼ��ʱ			
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);
				UART1_Rcvbuf[UART1_pRcv]= USART1->DR;
			
				UART1_pRcv++;		   	          		  //�����ֽ���Ŀ
			  if(UART1_pRcv>253)
				{UART1_pRcv=0;}	
				
		  	Uart1_Recving=1;
			  Uart1_Tick=0;
				Uart1_OV=0;
// 				if(UART1_pRcv==1)			             //�ж�ͬ��ͷ
// 					{
// 						if(UART1_Rcvbuf[0]!=0x7f)      //���Ծ������־
// 						UART1_pRcv=0;	
// 					}
// 					
// 				if(UART1_pRcv==2)				
// 					{
// 						UART1_Rcvlend=(UART1_Rcvbuf[1]+2);
// 					}	

			//	time2=0;
				if(UART1_pRcv==0xff)     //���������־
					{
						UART1_pRcv= 0 ;
						UART1_RcvOK=1;	
					}
		}
		
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART1,USART_FLAG_ORE);	//��SR
			USART_ReceiveData(USART1);				//��DR
		}
}

/*******************************************************************************
* �� �� ��         : void UART1_Put_Char(unsigned char DataToSend)
* ��������		     : RS232����һ���ֽ�
* ��    ��         : unsigned char DataToSend   Ҫ���͵��ֽ�����
* ��    ��         :
*******************************************************************************/
void UART1_Put_Char(unsigned char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART1�ķ��ͻ�����
	USART_SendData(USART1, (unsigned char) DataToSend);
	//�ȴ��������
  while (!(USART1->SR & USART_FLAG_TXE));
	//TxBuffer[count++] = DataToSend;  
  // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}

/*******************************************************************************
* �� �� ��         :  void UART1_Put(unsigned char *Str,unsigned char len)
* ��������		     : RS232����һ���ַ���
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
 void UART1_Put(unsigned char *Str,unsigned char len)
{
	 unsigned char i;
	 for(i=0;i<len;i++)
		{
			UART1_Put_Char(*Str);
			 Str++;
		}
}

/*******************************************************************************
* �� �� ��         :  void UART1_Put_String(unsigned char *Str)
* ��������		     :  RS232�����ַ���
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void UART1_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	//�ȴ��������.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Check_Uart1(void)
*��������:		������1���յ�����
���������
		
���������
*******************************************************************************/             
void Check_Uart1(void)
{
  if ( Uart1_OV == 1 ) // 10MS ʱ�䵽   				
  {
          Uart1_OV = 0;
          Data_UART();    //���ݴ���
          Watchdog();
  }
}

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//
//}; 
//
//FILE __stdout;       
//
//int fputc(int ch, FILE *f)
//{      
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//    USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif

/***********************************************************
* ����: PUTCHAR_PROTOTYPE
* ����: ���һ���ֽڵ�����
* ����: ��
* ����: ��
* �޸�:
* ע��: ���������iar��ʵ������putchar����(�ļ���ͷ����)��ʵ����putchar�������
�󣬱��������ܹ�ʹ��printf������
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1, (uint8_t) ch);
  
  while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE))
  {}
  
  return ch;
}

//------------------End of File----------------------------
