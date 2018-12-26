
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "iic2.h"
#include "delay.h"
/***********************************************************
* ����: IIC_GPIO_Config()
* ����: IIC��ʼ��
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
/********************������ٶȴ�����IIC��������**********************/
#ifdef ACCELERATION
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}
/***********************************************************
* ����: I2C_delay()
* ����: IIC��ʱ����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_delay(void)
{
    u8 i = 50; //��������Ż��ٶ�	����������͵�5����д��
    while(i)
    {
        i--;
    }
}
/***********************************************************
* ����: I2C_Start()
* ����: IIC��ʼ����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
int I2C_Start(void)
{
  SDA_H;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SDA_L;
  I2C_delay();
  SCL_L;
  I2C_delay();
  return 0;
}

/***********************************************************
* ����: Stop()
* ����: IICֹͣ����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_Stop(void)
{
  SCL_L;
  I2C_delay();
  SDA_L;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SDA_H;
  I2C_delay();
}

/***********************************************************
* ����: I2C_Ack()
* ����: ��������Ӧ���ź�
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_Ack(void)
{	
  SCL_L;
  I2C_delay();
  SDA_L;   //SDA�͵�ƽ
  I2C_delay();
  SCL_H; //SCL�ߵ�ƽ
  I2C_delay();
  SCL_L;
  I2C_delay();
}

/***********************************************************
* ����: I2C_NoAck()
* ����: ������ȷ���ź�
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_NoAck(void)
{	
  SCL_L;
  I2C_delay();
  SDA_H;
  I2C_delay();
  SCL_H;
  I2C_delay();
  SCL_L;
  I2C_delay();
}
/***********************************************************
* ����: I2C_WaitAck()
* ����: �ȴ����豸����Ӧ���ź�
* ����: ��
* ����: 1Ϊ��ACK,0Ϊ��ACK
* �޸�:
* ע��: 
***********************************************************/
int I2C_WaitAck(void) 	
{
  SCL_L;
  I2C_delay();
  SDA_H;			
  I2C_delay();
  SCL_H;
  I2C_delay();
  if(SDA_read)
  {
    SCL_L;
    return 1;
  }
  SCL_L;
  return 0;
}

/***********************************************************
* ����: I2C_SendByte()
* ����: ���Ͱ�λ����
* ����: ���� SendByte ��Ҫ���͵��ֽ�
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
  u8 i=8;
  while(i--)
  {
    SCL_L;
    I2C_delay();
    if(SendByte&0x80)
    {
      SDA_H;
    }
    else 
    {
      SDA_L;
    }
    SendByte<<=1;
    I2C_delay();
    SCL_H;
    I2C_delay(); 
  }
}

/***********************************************************
* ����: I2C_ReceiveByte()
* ����: ��ȡ��λ����
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
u8 I2C_ReceiveByte(void)  //���ݴӸ�λ����λ//
{ 
  u8 i=8;
  u8 ReceiveByte=0;
  SDA_H;
  while(i--)
  {
    ReceiveByte<<=1;      
    SCL_L;
    I2C_delay();
    SCL_H;//������ʱ SCLΪ�ߵ�ƽ
    I2C_delay();	
    if(SDA_read)
    {
      ReceiveByte|=0x01;
    }
  }
  return ReceiveByte;
}

