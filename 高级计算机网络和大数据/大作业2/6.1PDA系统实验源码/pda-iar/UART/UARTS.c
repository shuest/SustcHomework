/* 
---------硬件上的引脚连接:----------
RS232接口：
RS232TXD  -->  PA9  (UART1-TXD)
RS232RXD  -->  PA10 (UART1-RXD)
------------------------------------
 */


#include "UARTS.h"
#include "CmdLib.h"

//注意IAR要加上 _DLIB_FILE_DESCRIPTOR 宏定义
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

unsigned char UART1_pRcv,UART1_RcvOK;	//接收指针、接收完成标记 
unsigned char UART1_Rcvbuf[255]={00};

/*******************************************************************************
* 函 数 名         : void NVIC_Configuration(void)
* 函数功能		     : NVIC配置
* 输    入         : 
* 输    出         :
*******************************************************************************/
void NVIC_Configuration(void)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/*配置选中的中断向量*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/*设置了成员NVIC_IRQChannel中的先占优先级*/  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/*设置了成员NVIC_IRQChannel中的响应优先级*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/*使能中断向量*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/*调用库函数，初始化中断向量*/
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : void Initial_UART1(u32 baudrate)
* 函数功能		     : 初始化STM32-SDK开发板上的RS232接口
* 输    入         : u32 baudrate   设置RS232串口的波特率
* 输    出         :
*******************************************************************************/
void Initial_UART1(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能 UART1 模块的时钟  使能 UART1对应的引脚端口PA的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART1时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);  //端口复用
// 	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);                              //端口复用
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);                        //端口复用

/* 配置UART1 的发送引脚
	 配置PA9 为复用输出  刷新频率50MHz */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
/* 配置UART1 的接收引脚
	 配置PA10为浮地输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);														 
	  
/* 
	UART1的配置:
	1.波特率为调用程序指定的输入 baudrate;
	2. 8位数据			  USART_WordLength_8b;
	3.一个停止位			  USART_StopBits_1;
	4. 无奇偶效验			  USART_Parity_No ;
	5.不使用硬件流控制	  USART_HardwareFlowControl_None;
	6.使能发送和接收功能	  USART_Mode_Rx | USART_Mode_Tx;
 */
	USART_InitStructure.USART_BaudRate = baudrate;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;//禁用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送、接收使能
	
	//配置USART1时钟
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;  //时钟低电平活动
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;  //SLCK引脚上时钟输出的极性->低电平
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;  //时钟第二个边沿进行数据捕获
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
	
	//应用配置到UART1
	USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStruct);

	//使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能USART1
	USART_Cmd(USART1, ENABLE); 
	NVIC_Configuration();
	UART1_pRcv=0x00;
	UART1_RcvOK=0X00;
}

/*******************************************************************************
* 函 数 名         : void USART1_IRQHandler(void)
* 函数功能		     : 串口中断处理函数
* 输    入         : 
* 输    出         :
*******************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)                //接收中断 (接收寄存器非空) 
		{
			  
        START_TIME2(); // TIM2 开始计时			
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);
				UART1_Rcvbuf[UART1_pRcv]= USART1->DR;
			
				UART1_pRcv++;		   	          		  //接收字节数目
			  if(UART1_pRcv>253)
				{UART1_pRcv=0;}	
				
		  	Uart1_Recving=1;
			  Uart1_Tick=0;
				Uart1_OV=0;
// 				if(UART1_pRcv==1)			             //判断同步头
// 					{
// 						if(UART1_Rcvbuf[0]!=0x7f)      //不对就清除标志
// 						UART1_pRcv=0;	
// 					}
// 					
// 				if(UART1_pRcv==2)				
// 					{
// 						UART1_Rcvlend=(UART1_Rcvbuf[1]+2);
// 					}	

			//	time2=0;
				if(UART1_pRcv==0xff)     //接收溢出标志
					{
						UART1_pRcv= 0 ;
						UART1_RcvOK=1;	
					}
		}
		
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
		{
			USART_ClearFlag(USART1,USART_FLAG_ORE);	//读SR
			USART_ReceiveData(USART1);				//读DR
		}
}

/*******************************************************************************
* 函 数 名         : void UART1_Put_Char(unsigned char DataToSend)
* 函数功能		     : RS232发送一个字节
* 输    入         : unsigned char DataToSend   要发送的字节数据
* 输    出         :
*******************************************************************************/
void UART1_Put_Char(unsigned char DataToSend)
{
	//将要发送的字节写到UART1的发送缓冲区
	USART_SendData(USART1, (unsigned char) DataToSend);
	//等待发送完成
  while (!(USART1->SR & USART_FLAG_TXE));
	//TxBuffer[count++] = DataToSend;  
  // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
}

/*******************************************************************************
* 函 数 名         :  void UART1_Put(unsigned char *Str,unsigned char len)
* 函数功能		     : RS232发送一个字符串
* 输    入         : 
* 输    出         :
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
* 函 数 名         :  void UART1_Put_String(unsigned char *Str)
* 函数功能		     :  RS232发送字符串
* 输    入         : 
* 输    出         :
*******************************************************************************/
void UART1_Put_String(unsigned char *Str)
{
	//判断Str指向的数据是否有效.
	while(*Str){
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\r')UART1_Put_Char(0x0d);
		else if(*Str=='\n')UART1_Put_Char(0x0a);
			else UART1_Put_Char(*Str);
	//等待发送完成.
  	//while (!(USART1->SR & USART_FLAG_TXE));
	//指针++ 指向下一个字节.
	Str++;
	}
}

/**************************实现函数********************************************
*函数原型:		void Check_Uart1(void)
*功　　能:		处理串口1接收的数据
输入参数：
		
输出参数：
*******************************************************************************/             
void Check_Uart1(void)
{
  if ( Uart1_OV == 1 ) // 10MS 时间到   				
  {
          Uart1_OV = 0;
          Data_UART();    //数据处理
          Watchdog();
  }
}

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
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
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif

/***********************************************************
* 名称: PUTCHAR_PROTOTYPE
* 功能: 输出一个字节到串口
* 参数: 无
* 返回: 无
* 修改:
* 注释: 这个函数在iar中实际上是putchar函数(文件开头定义)，实现了putchar这个函数
后，编译器就能够使用printf函数。
***********************************************************/
PUTCHAR_PROTOTYPE
{
  USART_SendData(USART1, (uint8_t) ch);
  
  while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE))
  {}
  
  return ch;
}

//------------------End of File----------------------------
