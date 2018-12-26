#include "SHT10.h"
#include "stdio.h"
	
//温度转换结果
float TempResult;
//湿度转换结果
float HumResult;

////////////////////////////////////////////////////////////////////////////////// 	  
//初始化IIC
void SHT10_IOConfig(void)
{					     
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
  
    //PB2 SDA 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //PB1 SCK 推挽输出 sck不用输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
//PB0输入状态
void SDA_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA 上拉电阻输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}
//PB0输出状态
void SDA_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//写数据   写一个字节
u8 WriteByte(u8 value)	 
{
    u8 i, error = 0;
    SDA_Out();
    
    for (i = 0x80; i > 0; i /= 2)
    {
        if (i & value)
                SDA_H();
        else
                SDA_L();
        
        delay_us(1);
        SCK_H();
        delay_us(3);
        SCK_L();
    }
    
    SDA_In();//释放data线
    delay_us(2);	
    SCK_H();
    delay_us(1);
    
    //SDA_In();
    

    error = SDA_R();
    SCK_L();
    //SCK_H();
    SDA_Out();
    SDA_H();
    
    return error;
}

//读数据 一个字节
u8 ReadByte (u8 Ack)
{
    u8 i,val = 0;
    
    SDA_In();
    
    for (i = 0x80; i > 0; i>>=1)
    {
        delay_us(1);
        SCK_H();
        delay_us(1);
        if (SDA_R())
                val=(val | i);//读数据
        SCK_L();
    }
    
    SDA_Out();
    if (Ack)	   //判断是否应答 每8位校验
        SDA_L();
    else
        SDA_H();
    
    delay_us(1);
    SCK_H();
    delay_us(3);
    SCK_L();
    delay_us(3);
    
    return val;
}
//启动传输
void Start(void)	//正确
{
    SDA_Out();
    SDA_H();
    SCK_L();
    delay_us(1);
    SCK_H();
    delay_us(1);
    SDA_L();
    delay_us(1);
    SCK_L();
    delay_us(3);
    SCK_H();
    delay_us(1);
    SDA_H();
    delay_us(1);
    SCK_L();
    delay_us(1);
}
//连接复位
void ConReset (void) //正确
{
    u8 i;
    
    SDA_Out();
    delay_us(1);
    SDA_H();
    SCK_L();
    
    for (i = 0; i < 9; i++)
    {
            SCK_H();
            delay_us(1);
            SCK_L();
            delay_us(1);
    }
    Start();
}
//软复位
u8 SoftReset (void)
{
    u8 error = 0;
    ConReset();
    error += WriteByte(RESET);
    return error;
}
//测量温湿度
u8 Measure(u16 *pValue, u8 *pCheckSum, u8 mode)
{
    u8 error = 0;
    
    u8 Value_H = 0;
    u8 Value_L = 0;
    
    Start();
    
    switch(mode)
    {
        case TEMP : 
        {
            error += WriteByte(MEASURE_TEMP);
            while(error!=0)
            {	
// 				printf("Read Temp failed，wait..\n");
                ConReset();
                error += WriteByte(MEASURE_TEMP);	  
            }
// 			printf("Read Temp SUCCED\n");
            break;
        }
        case HUMI : 
        {
            error += WriteByte(MEASURE_HUMI);
            while(error!=0)
            {
// 				printf("Read RH failed，wait..\n");
                ConReset();
                error += WriteByte(MEASURE_HUMI);	    
            }
// 			printf("Read RH SUCCEED\n");
            break;
        }
        default:
            break;
    }
    SDA_In();

    delay_ms(320);
    while(SDA_R());//确保测量完毕
    Value_H = ReadByte(ACK);//读高位
    Value_L = ReadByte(ACK);//读低位
    
    *pCheckSum = ReadByte(noACK);
    
    *pValue = (Value_H << 8) | Value_L;	  //将高地位组合
    
    return error;
}

//计算温度和湿度
void Cal(u16 Temp, u16 Hum, float *pTempValue, float *pHumValue)
{
    const float d1 = -40.1;
    const float d2 = 0.01;
    float Temp_C;

    const float C1 = -2.0468;           
    const float C2 = +0.0367;           
    const float C3 = -0.0000015955;     
    const float T1 = +0.01;             
    const float T2 = +0.00008;  
    
    //湿度线性值
    float RH_Lin;
    //湿度真实值
    float RH_True;  
    
    //RH线性结果
    Temp_C = d1 + d2 *Temp; 
    RH_Lin = C1 + C2 *Hum + C3 *Hum *Hum;	//线性值
    RH_True = (Temp_C - 25)*(T1 + T2 *Hum) + RH_Lin; //补偿最终结果
    //限定范围
    if( RH_True > 100 ) RH_True = 100; 
    if( RH_True < 0.01) RH_True = 0.01;
    
    *pTempValue = Temp_C;	 //传回温度值
    *pHumValue = RH_True;	 //传回湿度值

}
void read_temperature_and_humidity(void)
{
    //温度结果  16bit
    u16 TempValue = 0;
    //湿度结果 16bit
    u16 HumValue = 0;

    //校验值
    u8 CheckValue = 0;// = 0x00;
    //错误
    u8 error = 0;// = 0x00;
		
    char Test_Timetick = 1;
    char Test_Counter = 0;
    if(Test_Timetick) 
    {
        Test_Timetick = 0;
        Test_Counter = 1;

        if(Test_Counter)
        {
            Test_Counter = 0;
            //SHT10 连接复位
            ConReset();
            //获得温度和湿度数据，16位格式
            error += Measure(&TempValue,&CheckValue,TEMP);
            error += Measure(&HumValue,&CheckValue,HUMI);
            //温度湿度计算，浮点数形式
            Cal(TempValue ,HumValue,&TempResult,&HumResult);
            //通过串口输出，温度和湿度数据

    //	printf("\r\nTemp %2.1fC RH %2.1f%%\r\n",TempResult,HumResult);	
        }
    }
}
