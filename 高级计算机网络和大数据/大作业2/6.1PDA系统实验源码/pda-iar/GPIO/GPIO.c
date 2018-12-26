#include "GPIO.h"
RCC_ClocksTypeDef RCC_ClockFreq;

extern unsigned  int Self_Test;

/*******************************************************************************
* �� �� ��         : void GPIO_Configuration(void)
* ��������		     : GPIO����
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  		  //����AFIOʱ��
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //��ֹJTAG����

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //�˿�ʱ��	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //�˿�ʱ��

  /*12864 GPIO �����˿�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  Delay_ms(10);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    	//50Mʱ���ٶ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*bmp085 �����˿�*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; //SDA 12   SCL 13      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	     //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //50Mʱ���ٶ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*���� GPIO �����˿�*/	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   AUTO
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   MANUAL
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   LOCK
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   UNLOCK
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   LOCK_CHECK
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   REMOVE_ALARM
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������   LIFT_ROD
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*LED �����˿�*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*������ �����˿�*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*��λ����*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOC,GPIO_Pin_1);
}

/*******************************************************************************
* �� �� ��         : void PDA_Init(void)
* ��������		     : PDA��ʼ��
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void PDA_Init(void)
{
  SystemInit();                       //����ϵͳʱ��Ϊ72M ʹ���ⲿ8M����+PLL
  SysTick_Init();
  RCC_GetClocksFreq(&RCC_ClockFreq);	//��ʼ��ʱ��Դ
  GPIO_Configuration();               //IO�ڳ�ʼ����LED��ʼ����������ʼ��
  I2C_Init1();
  AT24CXX_Init();											//EEprom��ʼ��
  Self_Test=AT24CXX_ReadLenByte(Reset_Flag_Addr,2);
  LCD12864_Init();                    //LCD12864��ʼ��	
  Delay_ms(10);                       //��ʱ	
  LORA_Pwor_H();
  LCD1264_clear();                    //LCD����	
  Delay_ms(10);                       //��ʱ
  LORA_SETA_H();
  LORA_SETB_H();
  Adc_Init(); 									      //AD��ʼ��
  Initial_UART1(57600);  
  Initial_UART4(9600);

  SHTXX_Init();      								  //��ʪ�ȳ�ʼ��
  bmp280Init(); 											//������ѹ��ʼ��
                                        
  LCD12864_wstr(2,2,"��̫�Ƽ�");
  LCD12864_wstr(3,1,"PDA  SYSTEM");
  Watchdog();
  Delay_ms(300);
  Watchdog();
  Delay_ms(300);
  Watchdog();
  Delay_ms(300);
  Watchdog();
        
  TIM2_NVIC_Configuration();  	 //TIM2 ��ʱ���� 
  TIM2_Configuration();

  TIM3_NVIC_Configuration();  	 //TIM3 ��ʱ���� 
  TIM3_Configuration();
        
  TIM4_NVIC_Configuration();     //TIM4 ��ʱ���� 
  TIM4_Configuration();
  Watchdog();
}

/*******************************************************************************
* �� �� ��         : void Watchdog(void)
* ��������		     : ���Ź�ι��
* ��    ��         : 
* ��    ��         :
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

