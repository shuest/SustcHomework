#include "SHT10.h"
#include "stdio.h"
	
//�¶�ת�����
float TempResult;
//ʪ��ת�����
float HumResult;

////////////////////////////////////////////////////////////////////////////////// 	  
//��ʼ��IIC
void SHT10_IOConfig(void)
{					     
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //ʹ��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
  
    //PB2 SDA �������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //PB1 SCK ������� sck��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
//PB0����״̬
void SDA_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA ������������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}
//PB0���״̬
void SDA_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //PB2 SDA �������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//д����   дһ���ֽ�
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
    
    SDA_In();//�ͷ�data��
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

//������ һ���ֽ�
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
                val=(val | i);//������
        SCK_L();
    }
    
    SDA_Out();
    if (Ack)	   //�ж��Ƿ�Ӧ�� ÿ8λУ��
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
//��������
void Start(void)	//��ȷ
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
//���Ӹ�λ
void ConReset (void) //��ȷ
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
//��λ
u8 SoftReset (void)
{
    u8 error = 0;
    ConReset();
    error += WriteByte(RESET);
    return error;
}
//������ʪ��
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
// 				printf("Read Temp failed��wait..\n");
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
// 				printf("Read RH failed��wait..\n");
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
    while(SDA_R());//ȷ���������
    Value_H = ReadByte(ACK);//����λ
    Value_L = ReadByte(ACK);//����λ
    
    *pCheckSum = ReadByte(noACK);
    
    *pValue = (Value_H << 8) | Value_L;	  //���ߵ�λ���
    
    return error;
}

//�����¶Ⱥ�ʪ��
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
    
    //ʪ������ֵ
    float RH_Lin;
    //ʪ����ʵֵ
    float RH_True;  
    
    //RH���Խ��
    Temp_C = d1 + d2 *Temp; 
    RH_Lin = C1 + C2 *Hum + C3 *Hum *Hum;	//����ֵ
    RH_True = (Temp_C - 25)*(T1 + T2 *Hum) + RH_Lin; //�������ս��
    //�޶���Χ
    if( RH_True > 100 ) RH_True = 100; 
    if( RH_True < 0.01) RH_True = 0.01;
    
    *pTempValue = Temp_C;	 //�����¶�ֵ
    *pHumValue = RH_True;	 //����ʪ��ֵ

}
void read_temperature_and_humidity(void)
{
    //�¶Ƚ��  16bit
    u16 TempValue = 0;
    //ʪ�Ƚ�� 16bit
    u16 HumValue = 0;

    //У��ֵ
    u8 CheckValue = 0;// = 0x00;
    //����
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
            //SHT10 ���Ӹ�λ
            ConReset();
            //����¶Ⱥ�ʪ�����ݣ�16λ��ʽ
            error += Measure(&TempValue,&CheckValue,TEMP);
            error += Measure(&HumValue,&CheckValue,HUMI);
            //�¶�ʪ�ȼ��㣬��������ʽ
            Cal(TempValue ,HumValue,&TempResult,&HumResult);
            //ͨ������������¶Ⱥ�ʪ������

    //	printf("\r\nTemp %2.1fC RH %2.1f%%\r\n",TempResult,HumResult);	
        }
    }
}
