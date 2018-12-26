#include "uart3.h"

unsigned char UART3_pRcv,UART3_RcvOK,UART3_Rcvlend;	//接收指针、接收完成标记 

unsigned char UART3_Rcvbuf[100]={00};

void U3NVIC_Configuration(void)
{
		/*定义一个NVIC_InitTypeDef 类型的结构体，名字叫NVIC_InitStructure*/ 
	NVIC_InitTypeDef NVIC_InitStructure; 
	  /**********************************************  
     NVIC_PriorityGroup_0：先占优先级0位从优先级4位
     NVIC_PriorityGroup_1：先占优先级1位从优先级3位
     NVIC_PriorityGroup_2：先占优先级2位从优先级2位
     NVIC_PriorityGroup_3：先占优先级3位从优先级1位
     NVIC_PriorityGroup_4：先占优先级4位从优先级0位
    ***********************************************/ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/*配置选中的中断向量*/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	/*设置了成员NVIC_IRQChannel中的先占优先级*/  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/*设置了成员NVIC_IRQChannel中的响应优先级*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	/*使能中断向量*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*调用库函数，初始化中断向量*/
  NVIC_Init(&NVIC_InitStructure);
}

void Initial_UART3(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //开启USART3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	
	/*Tx
		 配置PC10 为复用输出  刷新频率50MHz
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
	
	//配置USART3
	USART_InitStructure.USART_BaudRate = baudrate;        //波特率可以通过
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //9位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;    //                         使用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
	//配置USART3时钟
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
	
	USART_Init(USART3, &USART_InitStructure);
	USART_ClockInit(USART3, &USART_ClockInitStruct);

	//使能USART3接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//使能USART3
	USART_Cmd(USART3, ENABLE); 
	
	U3NVIC_Configuration();

}


void USART3_IRQHandler(void)
{

	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)                //接收中断 (接收寄存器非空) 
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);
			
				UART3_Rcvbuf[UART3_pRcv]= USART3->DR;
		
				UART3_pRcv++;		   	          		  //接收字节数目
				
		}
		
 	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART3,USART_FLAG_ORE);	//读SR
			USART_ReceiveData(USART3);				//读DR
		}
}

void UART3_Put_Char(unsigned char DataToSend)
{
	//将要发送的字节写到UART3的发送缓冲区
	USART_SendData(USART3, (unsigned char) DataToSend);
	//等待发送完成
  	while (!(USART3->SR & USART_FLAG_TXE));
	//TxBuffer[count++] = DataToSend;  
   // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}


/**************************实现函数********************************************
*函数原型:		void UART1_Put_String(unsigned char *Str)
*功　　能:		RS232发送字符串
输入参数：
		unsigned char *Str   要发送的字符串
输出参数：没有	
*******************************************************************************/
void UART3_Put_String(unsigned char *Str)
{
	//判断Str指向的数据是否有效.
	while(*Str){
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\r')UART3_Put_Char(0x0d);
		else if(*Str=='\n')UART3_Put_Char(0x0a);
			else UART3_Put_Char(*Str);
	//等待发送完成.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//指针++ 指向下一个字节.
	Str++;
	}
}
