#include "IIC.h"
#include "stm32f10x.h"
//��ʼ��IIC

void EEP_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_3;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructer);
}

void EEP_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_3;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructer);
}

void IIC_Init(void)
{					     
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
     
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  		//��©���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_SetBits(GPIOE,GPIO_Pin_2|GPIO_Pin_3); 
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
  SDA_OUT();     //sda�����
  IIC_SDA=1;	  	  
  IIC_SCL=1;
  Delay_us(4);
  IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
  Delay_us(4);
  IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	
  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
  EEP_SDA_OUT();//sda�����
  IIC_SCL=0;
  IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
  Delay_us(4);
  IIC_SCL=1; 
  IIC_SDA=1;//����I2C���߽����ź�
  Delay_us(4);						   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
  u8 ucErrTime=0;
  EEP_SDA_IN();      //SDA����Ϊ����  
  IIC_SDA=1;Delay_us(1);   
  IIC_SCL=1;Delay_us(1);	 
  while(EEP_READ_SDA)
  {
          ucErrTime++;
          if(ucErrTime>250)
          {
                  IIC_Stop();
                  return 1;
          }
  }
  IIC_SCL=0;//ʱ�����0 	   
  return 0;  
}
 
//����ACKӦ��
void IIC_Ack(void)
{
  IIC_SCL=0;
  EEP_SDA_OUT();
  IIC_SDA=0;
  Delay_us(2);
  IIC_SCL=1;
  Delay_us(2);
  IIC_SCL=0;
}

//������ACKӦ��		    
void IIC_NAck(void)
{
  IIC_SCL=0;
  EEP_SDA_OUT();
  IIC_SDA=1;
  Delay_us(2);
  IIC_SCL=1;
 Delay_us(2);
  IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
  EEP_SDA_OUT(); 	    
  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
  //IIC_SDA=(txd&0x80)>>7;
    if((txd&0x80)>>7)
            IIC_SDA=1;
    else
            IIC_SDA=0;
                      
    txd<<=1; 	  
    Delay_us(2);  //��TEA5767��������ʱ���Ǳ����
    IIC_SCL=1;
   Delay_us(2); 
    IIC_SCL=0;	
    Delay_us(2);
  }	 
}
 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  EEP_SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
  {
    IIC_SCL=0; 
   Delay_us(2);
    IIC_SCL=1;
    receive<<=1;
    if(EEP_READ_SDA)receive++;   
    Delay_us(1);
  }					 
  if (!ack)
    IIC_NAck();//����nACK
    else
    IIC_Ack(); //����ACK   
    return receive;
}






































