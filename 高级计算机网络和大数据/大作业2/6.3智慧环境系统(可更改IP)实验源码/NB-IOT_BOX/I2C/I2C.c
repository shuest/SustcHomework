#include "I2C.h"
#include "delay.h"

void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_5;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructer);
}

void I2C_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_5;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructer);
}

void I2C_Init1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  I2C_SCL1=1;
  I2C_SDA1=1;
}

void I2C_Start1(void)
{
  I2C_SDA_OUT();
  I2C_SDA1=1;
  I2C_SCL1=1;
  delay_us(20);  
  I2C_SDA1=0;
  delay_us(20);  
  I2C_SCL1=0;
  delay_us(20);       
}

void I2C_Stop1(void)
{
  I2C_SDA_OUT();
  I2C_SDA1=0;
  I2C_SCL1=1;
  delay_us(20);
  I2C_SDA1=1;
  delay_us(20);
}


void I2C_WaitAck1(void)
{
  u16 k;
  I2C_SDA_OUT();
  I2C_SCL1=0;
  I2C_SDA1=1;
  I2C_SDA_IN();
  delay_us(20);
  I2C_SCL1=1;
  k = 0;
  while((READ_SDA1!= 0) && (k < 60000))k++;
  delay_us(20);
  I2C_SCL1=0;
  delay_us(20);	
}

void I2C_WriteByte1(u8 byte)
{
  u8 i = 0;
  I2C_SDA_OUT();
  for(i = 0; i < 8; i++)
  {
   I2C_SCL1=0;
   delay_us(20);
   if(byte & 0x80)
   {
          I2C_SDA1=1;
   }
   else
   {
          I2C_SDA1=0;
   }
   delay_us(20);
   I2C_SCL1=1;
   delay_us(20);
   byte<<=1;
  }
   I2C_SCL1=0;
   delay_us(20);
}

u8 I2C_ReadByte1(void)
{
  u8 i,ReadByte;
  I2C_SDA_OUT();
  I2C_SDA1=1;
  I2C_SDA_IN();
  for(i = 0; i < 8; i++)
  {	
    ReadByte <<= 1;
    I2C_SCL1=0;
    delay_us(20);
    I2C_SCL1=1;
    delay_us(20);
    if(READ_SDA1)
    {
            ReadByte |= 0x01;
    }
    else
    {
            ReadByte &= ~(0x01);	
    }
                          
  }
  return ReadByte;
}


void I2C_Ack1(void)
{        
  I2C_SDA_OUT();
  I2C_SCL1=0;
  delay_us(20);
  I2C_SDA1=0;
  delay_us(20);
  I2C_SCL1=1;
  delay_us(20);
  I2C_SCL1=0;
  delay_us(20);
}

void I2C_NoAck1(void)
{        
  I2C_SDA_OUT();
  I2C_SCL1=0;
  delay_us(20);
  I2C_SDA1=1;
  delay_us(20);
  I2C_SCL1=1;
  delay_us(20);
  I2C_SCL1=0;
  delay_us(20);
}
