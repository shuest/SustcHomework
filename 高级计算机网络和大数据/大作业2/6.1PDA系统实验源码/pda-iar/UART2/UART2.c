#include "UART2.h"

unsigned char UART2_pRcv,UART2_RcvOK,UART2_Rcvlend;	//����ָ�롢������ɱ�� 

unsigned char UART2_Rcvbuf[100]={00};

// unsigned char Uart2_Get_Data;	//����2���յ�����

void U2NVIC_Configuration(void)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*����ѡ�е��ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	/*�����˳�ԱNVIC_IRQChannel�е���ռ���ȼ�*/  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/*�����˳�ԱNVIC_IRQChannel�е���Ӧ���ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/*ʹ���ж�����*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*���ÿ⺯������ʼ���ж�����*/
  NVIC_Init(&NVIC_InitStructure);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void Initial_UART2(u32 baudrate)
*��������:		��ʼ��STM32-SDK�������ϵ�RS232�ӿ�
���������
		u32 baudrate   ����RS232���ڵĲ�����
���������û��	
*******************************************************************************/
void Initial_UART2(u32 baudrate)
{
	
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�� UART2 ģ���ʱ��  ʹ�� UART2��Ӧ�����Ŷ˿�PA��ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //����USART2ʱ��
	
	//Tx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	//Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	

	USART_InitStructure.USART_BaudRate = baudrate;       //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART2ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	//ʹ��USART2�����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//ʹ��USART2
	USART_Cmd(USART2, ENABLE); 

	U2NVIC_Configuration();
	
	UART2_Rcvlend=0x00;
	UART2_pRcv=0x00;
}


void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)                //�����ж� (���ռĴ����ǿ�) 
		{
			  START_TIME3();  // TIM3 ��ʼ��ʱ
			
				USART_ClearITPendingBit(USART2,USART_IT_RXNE);  //�����ж�
	
				UART2_Rcvbuf[UART2_pRcv]= USART2->DR;
		
				UART2_pRcv++;		   	          		  //�����ֽ���Ŀ
				
		}
		
 	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART2,USART_FLAG_ORE);	//��SR
			USART_ReceiveData(USART2);				//��DR
		}
}

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



// int fputc(int ch, FILE *f)
// {
// /* ��Printf���ݷ������� */
//   USART_SendData(USART2, (unsigned char) ch);
//   while (!(USART2->SR & USART_FLAG_TXE));
//  
//   return (ch);
// }





