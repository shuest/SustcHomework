#include "delay.h"
#include "bmp085.h"
#define dev_addr_r  0xef//读寄存器地址
#define BMP085_SLAVE_ADDR  0xee//写寄存器地址
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


void delay(unsigned int k);
int16_t BMP085_Read_2B(uint8_t addr);
uint32_t BMP085_Read_3B(uint8_t addr);
long BMP085_Read_TEMP(void);
void delay(unsigned int k);
long BMP085_Read_Pressure(void);


void I2C2_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
/********************大气压力传感器IIC引脚配置**********************/
  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG  使PB3 PB4 PA15为IO口*/
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;             
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIOB->BSRR = GPIO_Pin_5;
//GPIOB->BRR = GPIO_Pin_5;

  
}

void I2C2_Start(void)
{   
  SDA_H;    
  SCL_H;   
  delay_us(10);
  SDA_L;   
  delay_us(6);//大于4个微秒
  SCL_L;
}


void I2C2_Stop(void)
{
    
  SCL_L;   
  SDA_L;   
  SCL_H;
  delay_us(5);//大于4个微秒  
  SDA_H;
  delay_us(5);//大于4个微秒
}

void Sendack(uint8_t h)
{
    
  SCL_L;
  if(h==0)
    SDA_L;//有应答信号
  else
    SDA_H;    
  delay_us(5);
  SCL_H;   
  delay_us(10);    
  SCL_L;

}



uint8_t I2C2_Check_ack(void)

{
  uint8_t count = 0;
  SCL_L;   
  SDA_H;//要读低电平需先拉高再读,否则读到的是错误数据,很重要！
  delay_us(5);  
  SCL_H;
  delay_us(5);  
  
  while(SDA_read)
  {
    count++;
    if(count > 250)
    {
      SCL_L;   
      delay_us(5);
      return 1;
    }
  }
  SCL_L;   
  delay_us(5);
  return 0;

}



uint8_t I2C2_Write_Byte(uint8_t b)

{
    
  uint8_t e=8;
    
  while(e--)
  {
        
    SCL_L;
    delay_us(5);    
    if(b&0x80)
      SDA_H;      
    else 
      SDA_L;    
    delay_us(5);
    b<<=1;      
    SCL_H;
    delay_us(5);
  }
    
  SCL_L;
  delay_us(5);  
  return(I2C2_Check_ack());

}



uint8_t I2C2_Read_Byte(void)

{
    
  uint8_t i=8;   
  uint8_t c=0;   
  SCL_H;
  delay_us(5);   
  while(i--)
  {       
    c<<=1;        
          
    if(SDA_read)
      c|=0x01;       
    else c&=0xfe;   
    delay_us(5);
    SCL_L;       
    delay_us(5);      
    SCL_H; 
    delay_us(5);
  }   
  SCL_L;   
  return c;

}



void I2C2_Write(uint8_t Slave_Addr, uint8_t REG_Address,uint8_t REG_data)

{
    
  I2C2_Start();
    
  I2C2_Write_Byte(Slave_Addr);
    
  I2C2_Write_Byte(REG_Address);
    
  I2C2_Write_Byte(REG_data);
    
  I2C2_Stop();

}


int16_t BMP085_Read_2B(uint8_t addr)
{
    uint8_t msb = 0, lsb = 0;
    I2C2_Start();//起始信号
    if(I2C2_Write_Byte(BMP085_SLAVE_ADDR) == 0)//发送设备地址+写信号
    {
      I2C2_Write_Byte(addr);//发送存储单元地址
      I2C2_Start();//起始信号
      I2C2_Write_Byte(dev_addr_r);//发送设备地址+读信号
      msb=I2C2_Read_Byte();
      Sendack(0);
      lsb=I2C2_Read_Byte();
      Sendack(1);
      I2C2_Stop();
    }
    return (short)((msb << 8) | lsb);
}

/*
uint32_t BMP085_Read_3B(uint8_t addr)
{
    uint8_t msb, lsb, xlsb;
    //uint32_t up = 0;
    I2C2_Start();
    if(I2C2_Write_Byte(BMP085_SLAVE_ADDR) == 0)
    {
      I2C2_Write_Byte(addr);
      I2C2_Start();
      I2C2_Write_Byte(BMP085_SLAVE_ADDR+1);
      msb=I2C2_Read_Byte();//读高位
      Sendack(0);
      lsb=I2C2_Read_Byte();//读中位
      Sendack(0);//ACK
      xlsb=I2C2_Read_Byte();//读低位
      Sendack(1);//NACK
      I2C2_Stop();
    }
    return (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);
}*/
                    
long BMP085_Read_TEMP(void)
{
    //int16_t temp;
    I2C2_Write(BMP085_SLAVE_ADDR, 0xF4, 0x2E);
    delay_us(10000);
    return (long)BMP085_Read_2B(0xF6);
}

long BMP085_Read_Pressure(void)
{
    //long pressure;
    I2C2_Write(BMP085_SLAVE_ADDR, 0xF4, (0x34 + (OSS << 6)));
    delay_us(10000);
    return ((long)(BMP085_Read_2B(0xF6)&0x0000ffff));
}

void Init_BMP085(void)
{
    I2C2_GPIO_Config();
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

void Multiple_Read_BMP085(long *press,uint16_t *temp)
{
    long ut;
    long up;
    long x1, x2, b5, b6, x3, b3, p,b7;
    unsigned long b4;
    
    long pressure;
    long temperature;
    
    ut = BMP085_Read_TEMP();
    up = BMP085_Read_Pressure();

    x1 = (((int32_t)ut - ac6) * ac5) >> 15;
    x2 = ((int32_t)mc << 11) / (x1 + md);
    b5 = x1 + x2;
    temperature = ((b5 + 8) >> 4);
    

    //dat->press = BMP085_Read_Pressure();
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
    *press = pressure;
    *temp = temperature;
}
