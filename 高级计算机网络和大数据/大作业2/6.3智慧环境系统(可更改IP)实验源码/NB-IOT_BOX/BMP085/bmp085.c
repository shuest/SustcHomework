#include "bmp085.h"
#include "delay.h"



#define BMP085_R_ADDR  0xef  //读寄存器地址
#define BMP085_W_ADDR  0xee  //写寄存器地址
#define OSS 0

int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;


int32_t BMP085_Pressure = 0;  //BMP085_气压数据暂存
int32_t BMP085_Temp = 0;      //BMP085_温度


//初始化IIC
void BMP085_IOConfig(void)
{					     
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
  
    //PB2 SDA 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //PB1 SCK 推挽输出 sck不用输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

//PB0输入状态
void BMP_SDA_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA 上拉电阻输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}
//PB0输出状态
void BMP_SDA_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : I2C_Start
* 函数功能		     : I2C 启动函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void I2C_Start(void)
{   
  BMP_SDA_H;    
  BMP_SCL_H;   
  delay_us(10);
  BMP_SDA_L;    
  BMP_SCL_L;
}

/*******************************************************************************
* 函 数 名         : I2C_Stop
* 函数功能		     : I2C停止函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void I2C_Stop(void)
{   
  BMP_SCL_L;   
  BMP_SDA_L;   
  BMP_SCL_H;  
  BMP_SDA_H;
}
/*******************************************************************************
* 函 数 名         : Sendack
* 函数功能		     : I2C发送
* 输    入         : h
* 输    出         : 无
*******************************************************************************/
void Sendack(uint8_t h)
{
  
  BMP_SCL_L;
  if(h==0)
    BMP_SDA_L;
  else
    BMP_SDA_H;    
  BMP_SCL_H;   
  delay_us(10);  
  BMP_SCL_L;
}


/*******************************************************************************
* 函 数 名         : I2C_Check_ack
* 函数功能		     : I2C ACK函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
uint8_t I2C_Check_ack(void)
{  
  BMP_SCL_L;  
  BMP_SDA_H;  
  BMP_SCL_H; 
  BMP_SDA_In();
  if(BMP_SDA_read==1) 
  {     
    BMP_SCL_L;   
    return 0; 
  }  
  BMP_SDA_Out();
  BMP_SCL_L;  
  return 1;
}

/*******************************************************************************
* 函 数 名         : I2C_Write_Byte
* 函数功能		     : I2C 写一个字节
* 输    入         : b
* 输    出         : 无
*******************************************************************************/
void I2C_Write_Byte(uint8_t b)
{  
  uint8_t e=8; 
  while(e--)
  {     
    BMP_SCL_L;    
    if(b&0x80)
      BMP_SDA_H;   
    else 
      BMP_SDA_L;   
    b<<=1;   
    BMP_SCL_H; 
  }   
  BMP_SCL_L; 
  I2C_Check_ack();
}


/*******************************************************************************
* 函 数 名         : I2C_Read_Byte
* 函数功能		     : I2C 读取一个字节
* 输    入         : b
* 输    出         : 无
*******************************************************************************/
uint8_t I2C_Read_Byte(void)
{  
  uint8_t i=8;   
  uint8_t c=0; 
  BMP_SCL_L; 
  BMP_SDA_H; 
  BMP_SDA_In();
  while(i--)
  {     
    c<<=1;   
    BMP_SCL_L;   
    delay_us(10);  
    BMP_SCL_H;   
    if(BMP_SDA_read==1)c|=0x01;    
    else c&=0xfe;
  } 
  BMP_SDA_Out();
  BMP_SCL_L;  
  return c;
}

/*******************************************************************************
* 函 数 名         : I2C_Write
* 函数功能		     : I2C 写寄存器数据
* 输    入         : 输入地址、数据
* 输    出         : 无
*******************************************************************************/
void I2C_Write(uint8_t Slave_Addr, uint8_t REG_Address,uint8_t REG_data)
{  
  I2C_Start();  
  I2C_Write_Byte(Slave_Addr); 
  I2C_Write_Byte(REG_Address);
  I2C_Write_Byte(REG_data);  
  I2C_Stop();
}

/*******************************************************************************
* 函 数 名         : BMP085_Read_2B
* 函数功能		     : I2C 读两个字节数据
* 输    入         : 输入地址、数据
* 输    出         : 无
*******************************************************************************/
int16_t BMP085_Read_2B(uint8_t addr)
{
  uint8_t msb, lsb;
  I2C_Start();
  I2C_Write_Byte(BMP085_W_ADDR);
  I2C_Write_Byte(addr);
  I2C_Start();
  I2C_Write_Byte(BMP085_R_ADDR);

  msb=I2C_Read_Byte();
  Sendack(0);
  lsb=I2C_Read_Byte();
  Sendack(1);
  I2C_Stop();
  return (int16_t)((msb << 8) | lsb);
}
/*******************************************************************************
* 函 数 名         : BMP085_Read_3B
* 函数功能		     : I2C 读两个字节数据
* 输    入         : 输入地址、数据
* 输    出         : 无
*******************************************************************************/
uint32_t BMP085_Read_3B(uint8_t addr)
{
  uint8_t msb, lsb, xlsb;
  I2C_Start();
  I2C_Write_Byte(BMP085_W_ADDR);
  I2C_Write_Byte(addr);
  I2C_Start();
  I2C_Write_Byte(BMP085_R_ADDR);
  msb=I2C_Read_Byte();
  Sendack(0);
  lsb=I2C_Read_Byte();
  Sendack(0);
  xlsb=I2C_Read_Byte();
  Sendack(1);
  I2C_Stop();
  return (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);
}
/*******************************************************************************
* 函 数 名         : BMP085_Read_TEMP
* 函数功能		     : I2C 读温度
* 输    入         : 
* 输    出         : 
*******************************************************************************/
int16_t BMP085_Read_TEMP(void)
{
  I2C_Write(BMP085_W_ADDR, 0xF4, 0x2E);
  delay_us(5000);
  return (int16_t)BMP085_Read_2B(0xF6);
}
/*******************************************************************************
* 函 数 名         : BMP085_Read_Pressure
* 函数功能		     : I2C 读湿度
* 输    入         : 
* 输    出         : 
*******************************************************************************/
int32_t BMP085_Read_Pressure(void)
{
  I2C_Write(BMP085_W_ADDR, 0xF4, (0x34 + (OSS << 6)));
  delay_us(5000);
  return ((int32_t)BMP085_Read_3B(0xF6));
}


/*******************************************************************************
* 函 数 名         : Init_BMP085
* 函数功能		     : 初始化
* 输    入         : 
* 输    出         : 
*******************************************************************************/
void Init_BMP085(void)
{
 // BMP085_IOConfig();
  ac1 = BMP085_Read_2B(0xAA);
  ac2 = BMP085_Read_2B(0xAC);
  ac3 = BMP085_Read_2B(0xAE);
  ac4 = BMP085_Read_2B(0xB0);
  ac5 = BMP085_Read_2B(0xB2);
  ac6 = BMP085_Read_2B(0xB4);
  b1 =  BMP085_Read_2B(0xB6);
  b2 =  BMP085_Read_2B(0xB8);
  mb =  BMP085_Read_2B(0xBA);
  mc =  BMP085_Read_2B(0xBC);
  md =  BMP085_Read_2B(0xBE);
}
/*******************************************************************************
* 函 数 名         : Multiple_Read_BMP085
* 函数功能		     : 读温湿度、气压
* 输    入         : 
* 输    出         : 
*******************************************************************************/
void Multiple_Read_BMP085(void)
{
  int32_t ut;
  int32_t up;
  int32_t x1, x2, b5, b6, x3, b3, p,b7;
  uint32_t b4;
  
  int32_t pressure;
  int32_t temperature;

  ut = BMP085_Read_TEMP();

  up = BMP085_Read_Pressure();
  

  x1 = (((int32_t)ut - ac6) * ac5) >> 15;
  x2 = ((int32_t)mc << 11) / (x1 + md);
  b5 = x1 + x2;
  temperature = ((b5 + 8) >> 4);
  

  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6) >> 12) >> 11;
  x2 = (ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t)ac1) * 4 + x3) << OSS) + 2) >> 2;
  x1 = (ac3 * b6) >> 13;
  x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)up - b3) * (50000 >> OSS);
  if( b7 < 0x80000000)
    p = (b7 * 2) / b4 ;
  else
    p = (b7 / b4) * 2;
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  pressure = p + ((x1 + x2 + 3791) >> 4);
	
  BMP085_Pressure = pressure;
  BMP085_Temp = temperature;
}
