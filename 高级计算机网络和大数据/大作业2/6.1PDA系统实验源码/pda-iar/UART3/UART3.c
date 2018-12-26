#include "uart3.h"

unsigned char UART3_pRcv,UART3_RcvOK,UART3_Rcvlend;	//����ָ�롢������ɱ�� 

unsigned char UART3_Rcvbuf[100]={00};

void U3NVIC_Configuration(void)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/*����ѡ�е��ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/*�����˳�ԱNVIC_IRQChannel�е���ռ���ȼ�*/  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/*�����˳�ԱNVIC_IRQChannel�е���Ӧ���ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/*ʹ���ж�����*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*���ÿ⺯������ʼ���ж�����*/
  NVIC_Init(&NVIC_InitStructure);
}

void Initial_UART3(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	
	/*Tx
		 ����PC10 Ϊ�������  ˢ��Ƶ��50MHz
	*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	//Rx
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
	//����USART3
	USART_InitStructure.USART_BaudRate = baudrate;        //�����ʿ���ͨ��
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //9λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //                         ʹ����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	//����USART3ʱ��
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //ʱ�ӵ͵�ƽ�
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK������ʱ������ļ���->�͵�ƽ
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //ʱ�ӵڶ������ؽ������ݲ���
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ClockInit(USART3, &USART_ClockInitStruct);

	//ʹ��USART3�����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//ʹ��USART3
	USART_Cmd(USART3, ENABLE); 
	
	U3NVIC_Configuration();

}


void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)                //�����ж� (���ռĴ����ǿ�) 
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);
			
				UART3_Rcvbuf[UART3_pRcv]= USART3->DR;
		
				UART3_pRcv++;		   	          		  //�����ֽ���Ŀ
				
		}
		
 	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART3,USART_FLAG_ORE);	//��SR
			USART_ReceiveData(USART3);				//��DR
		}
}

void UART3_Put_Char(unsigned char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART3�ķ��ͻ�����
	USART_SendData(USART3, (unsigned char) DataToSend);
	//�ȴ��������
  	while (!(USART3->SR & USART_FLAG_TXE));
	//TxBuffer[count++] = DataToSend;  
   // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		void UART1_Put_String(unsigned char *Str)
*��������:		RS232�����ַ���
���������
		unsigned char *Str   Ҫ���͵��ַ���
���������û��	
*******************************************************************************/
void UART3_Put_String(unsigned char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r')UART3_Put_Char(0x0d);
		else if(*Str=='\n')UART3_Put_Char(0x0a);
			else UART3_Put_Char(*Str);
	//�ȴ��������.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//ָ��++ ָ����һ���ֽ�.
	Str++;
	}
}
