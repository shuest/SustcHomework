
#include "shtxx.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define noACK 0
#define ACK   1

#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1

/***********************************************************
* 名称: IIC_GPIOConfig()
* 功能: IIC GPIO初始化
* 参数: 无
* 返回: 无
* 修改:
***********************************************************/
void IIC_GPIOConfig(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(SDA_GPIO_CLK | SCL_GPIO_CLK, ENABLE);
    //PB0 PB5
    GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN | SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}
/***********************************************************
* 名称: SDA_OUT()
* 功能: SDA引脚设置成推挽输出
* 参数: 无
* 返回: 无
* 修改:
***********************************************************/
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}
/***********************************************************
* 名称: SDA_IN()
* 功能: SDA引脚设置成上拉输入
* 参数: 无
* 返回: 无
* 修改:
***********************************************************/
void SDA_IN(void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
}
/***********************************************************
* 名称: Delay1us()
* 功能: 延时1us函数
* 参数: 输入 需要延时的微秒数
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
static void Delay1us(uint16_t t)
{
    uint8_t i, j;
    for (j = t; j > 0; j--)
        for (i = 10; i > 0; i--);
}

/***********************************************************
* 名称: SHTXX_Init()
* 功能: 温湿度传感器初始化函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void SHTXX_Init(void)
{
    IIC_GPIOConfig();
}

/***********************************************************
* 名称: ConnectionReset()
* 功能: 重置链接
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void ConnectionReset(void)
{
    uint8_t i;
    SDA_OUT();
    Set_IIC_SDA();
    Delay1us(10);
    Clr_IIC_SCL();
    Delay1us(10);
    for(i = 0; i < 9; i++){
        Set_IIC_SCL();
        Delay1us(10);
        Clr_IIC_SCL();
        Delay1us(10);
    }
    TransStart();
}

/***********************************************************
* 名称: WriteByte()
* 功能: 写入1字节命令
* 参数: 输入 value 需要写入的命令的数据
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
static char WriteByte(unsigned char value)
{
    unsigned char i, error = 0;
    SDA_OUT();
    for (i=0x80;i>0;i/=2)             //shift bit for masking
    { 
      if (i & value) 
        Set_IIC_SDA();          //masking value with i , write to SENSI-BUS
      else 
        Clr_IIC_SDA();
    Delay1us(10);
    Set_IIC_SCL();                          //clk for SENSI-BUS
    Delay1us(10);        //pulswith approx. 5 us  	
    Clr_IIC_SCL();
    Delay1us(10);
  }
  SDA_IN();
  Set_IIC_SDA();                           //release DATA-line
  Delay1us(10);
  Set_IIC_SCL();                            //clk #9 for ack 
  Delay1us(10);
  error=READ_SDA();                       //check ack (DATA will be pulled down by SHT11)
  Clr_IIC_SCL();        
  return error;                     //error=1 in case of no acknowledge
}


/***********************************************************
* 名称: ReadByte()
* 功能: 读取1字节数据
* 参数: 输入 ack ack=1表示需要向MMA传输停止位，为0则表示不需
*       要向MMA传输停止位
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
static char ReadByte(unsigned char ack)
{ 
  unsigned char i,val=0;

  Set_IIC_SDA();   //release DATA-line
  Delay1us(10);
  SDA_IN();
  for (i=0x80;i>0;i/=2)             //shift bit for masking
  { 
    Set_IIC_SCL();  //clk for SENSI-BUS
    Delay1us(10);
    if (READ_SDA()) val=(val | i);        //read bit  
    Clr_IIC_SCL();
    Delay1us(10);
  }
  SDA_OUT();
  WRITE_SDA(!ack);                 //in case of "ack==1" pull down DATA-Line
  Delay1us(10);
  Set_IIC_SCL();   //clk #9 for ack
  Delay1us(10);
  Clr_IIC_SCL();
  Delay1us(10);
  Set_IIC_SDA();                           //release DATA-line
  Delay1us(10);
  return val;
}
/***********************************************************
* 名称: TransStart()
* 功能: 开始传送数据
* 参数: 无
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
static void TransStart(void)
{
    SDA_OUT();
    Set_IIC_SDA();
    Delay1us(10);
    Clr_IIC_SCL();
    Delay1us(10);
    Set_IIC_SCL();
    Delay1us(10);
    Clr_IIC_SDA();
    Delay1us(10);
    Clr_IIC_SCL();
    Delay1us(10);
    Set_IIC_SCL();
    Delay1us(10);
    Set_IIC_SDA();
    Delay1us(10);
    Clr_IIC_SCL();
    Delay1us(10);
}
/***********************************************************
* 名称: Measure()
* 功能: 得到测量的值
* 参数: 输出 p_value 输出的数据
        输出 p_checksum 输出的数据校验值
        输入 模式 有温度模式和湿度模式
* 返回: 错误的值 0为无错误
* 修改:
* 注释: 
***********************************************************/
uint8_t Measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
{
    unsigned error = 0;
    //TransStart();
    ConnectionReset();
    switch(mode){
    case TEMP: error = WriteByte(MEASURE_TEMP);break;
    case HUMI: error = WriteByte(MEASURE_HUMI);break;
    default: break;
    }
    SDA_IN(); 
    error = 0;
    while(error++<400000 && READ_SDA()); //wait until sensor has finished the measurement
    if(READ_SDA())  {
      //error+=1;                // or timeout (~2 sec.) is reached
      return 1;
    }
    *(p_value +1)  =ReadByte(ACK);    //read the first byte (MSB)
    *(p_value)=ReadByte(ACK);    //read the second byte (LSB)
    *p_checksum =ReadByte(noACK);  //read checksum
    return 0;
}
/***********************************************************
* 名称: SHTXX_Cal()
* 功能: 转化得到的温湿度的值为标准单位额
* 参数: 输入 temp 得到的温度的值
        输入 p_checksum 得到的湿度的值
        输出 f_temp 转化后的温度的值
        输出 f_humi 转化后的温度的值
* 返回: 无
* 修改:
* 注释: 
***********************************************************/
void SHTXX_Cal(uint16_t *temp, uint16_t *humi, float *f_temp, float *f_humi )
{

  const float C1=-4.0;              // for 12 Bit
  const float C2=+0.0405;           // for 12 Bit
  const float C3=-0.0000028;        // for 12 Bit
  const float T1=+0.01;             // for 14 Bit @ 5V
  const float T2=+0.00008;           // for 14 Bit @ 5V	

  float rh;             // rh:      Humidity [Ticks] 12 Bit 
  float t;           // t:       Temperature [Ticks] 14 Bit
  float rh_lin;                     // rh_lin:  Humidity linear
  float rh_true;                    // rh_true: Temperature compensated humidity
  float t_C;                        // t_C   :  Temperature  

  rh = (float)(*humi);
  t = (float)(*temp);
  
  t_C=t*0.01 - 40;                  //calc. temperature from ticks 
  rh_lin=C3*rh*rh + C2*rh + C1;     //calc. humidity from ticks to [%RH]
  rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;   //calc. temperature compensated humidity [%RH]
  if(rh_true>100)rh_true=100;       //cut if the value is outside of
  if(rh_true<0.1)rh_true=0.1;       //the physical possible range

  *f_temp=t_C;               //return temperature 
  *f_humi=rh_true;              //return humidity[%RH]
  
}