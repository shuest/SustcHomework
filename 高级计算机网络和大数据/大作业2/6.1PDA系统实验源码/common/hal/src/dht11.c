#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stimer.h"

#define COM_SET     GPIOB->BSRR = GPIO_Pin_1
#define COM_CLR     GPIOB->BRR  = GPIO_Pin_1

#define COM_R       (GPIOB->IDR  & GPIO_Pin_1)

void DHT11_Rst(void);
uint8_t DHT11_Init(void);
u8 DHT11_Check(void) ;

/*********************add by guest**********************************/
extern void stimer_set(struct stimer *t, unsigned long interval);
extern void stimer_restart(struct stimer *t);
extern int stimer_expired(struct stimer *t);
/*********************add by guest**********************************/

void dht11_init(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    COM_SET;
}



#pragma optimize=none
static char dht11_read_bit(void)
{
  int i = 0;
  
  while (!COM_R);
  for (i=0; i<200; i++) {
    if (COM_R == 0) break;
  }
  if (i<30)return 0;  //30us
  return 1;
}

#pragma optimize=none
static unsigned char dht11_read_byte(void)
{
  unsigned char v = 0, b;
  int i;
  for (i=7; i>=0; i--) {
    b = dht11_read_bit();
    v |= b<<i;
  }
  return v; 
}



int dht11_read(unsigned int *t, unsigned int *h)
{
            unsigned int flag;
            unsigned char dat1, dat2, dat3, dat4, dat5, ck;
            
   
	    //主机拉低18ms 

            //COM_CLR;
	    //delay_ms(20);
	    //COM_SET;
            //delay_us(30);
            
//            while (DHT11_Init()) {
//              delay_ms(1000);
//            }
           
            
            flag = 20000;
	    //while (COM_R && --flag);
            //if (flag == 0) {
            //  return -1;
            //}
            if (DHT11_Check()) {
              return -1;
            }
	    //总线由上拉电阻拉高 主机延时20us
	    //主机设为输入 判断从机响应信号  
	    //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  	    
            flag = 2000;
            while (!COM_R && --flag);
            if (flag == 0) return -1;
            flag = 2000;
            while (COM_R && --flag);
            if (flag == 0) return -1;
                  
             
             dat1 = dht11_read_byte();

             dat2 = dht11_read_byte();

             dat3 = dht11_read_byte();
              
    
             dat4 = dht11_read_byte();
              
   
             dat5 = dht11_read_byte();           
                  
              
             ck = dat1 + dat2 + dat3 + dat4;
              
             if (ck == dat5) {
                *t = dat3;
                *h = dat1;   
                return 0;
             }
    return -1;
}




///////////////////////////////////////////////////////////////////////////////

static GPIO_InitTypeDef GPIO_InitStructure;

//IO方向设置
static void DHT11_IO_IN(void)
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
static void DHT11_IO_OUT(void)
{
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void DHT11_DQ_OUT(uint8_t dat)
{
  GPIO_WriteBit(GPIOB, GPIO_Pin_1, dat);
}

static uint8_t DHT11_DQ_IN(void)
{
  return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
}

//复位DHT11
void DHT11_Rst(void)	   
{                 
  DHT11_IO_OUT(); 	//SET OUTPUT
  DHT11_DQ_OUT(0); 	//拉低DQ
  delay_ms(20);    	//拉低至少18ms
  DHT11_DQ_OUT(1); 	//DQ=1 
  delay_us(30);     	//主机拉高20~40us
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
  u8 retry=0;
  DHT11_IO_IN();//SET INPUT	 
  while (DHT11_DQ_IN()&&retry<100)//DHT11会拉低40~80us
  {
    retry++;
    delay_us(1);
  };	 
  if(retry>=100)return 1;
  else retry=0;
  while (!DHT11_DQ_IN()&&retry<100)//DHT11拉低后会再次拉高40~80us
  {
    retry++;
    delay_us(1);
  };
  if(retry>=100)return 1;	    
  return 0;
}
//从DHT11读取一个位
//返回值：1/0
static u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN()&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN()&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN())return 1;
	else return 0;		   
}
//从DHT11读取一个字节
//返回值：读到的数据
static u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
int DHT11_Read_Data(unsigned char *temp,unsigned char *humi)    
{        
 	u8 buf[5];
	u8 i;
        
        
        static unsigned int t, h;
        static struct stimer timer;
        if (timer.interval == 0) {
          stimer_set(&timer, 2);
        }
        if (!stimer_expired(&timer)) {
          *temp = t;
          *humi = h;
          return 0;
        }
        stimer_restart(&timer);
            
	DHT11_Rst();
	if(DHT11_Check()) {
          return -1;
        }
	
	for(i=0;i<5;i++)//读取40位数据
	{
		buf[i]=DHT11_Read_Byte();
	}
	if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
	{
                h=buf[0];
                t=buf[2];
                
		*humi=h;
		*temp=t;
	}
	
	return 0;	    
}
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在     	 
uint8_t DHT11_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);				 
}



