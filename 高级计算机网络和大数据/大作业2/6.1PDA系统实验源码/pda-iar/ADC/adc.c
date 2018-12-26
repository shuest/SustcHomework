 #include "adc.h"
//#include "delay.h"
		   		
/*******************************************************************************
* 函 数 名         : void  Adc_Init(void) 
* 函数功能		     : 初始化ADC，ch:通道默认值 0~3
* 输    入         : 
* 输    出         :
*******************************************************************************/
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   						//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
                        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	ADC_DeInit(ADC1);  															//复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 12;														//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);																	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);											//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);									//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);								  //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 		//等待校准结束
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}				  

/*******************************************************************************
* 函 数 名         : u16 Get_Adc(u8 ch) 
* 函数功能		     : 获得ADC值，ch:通道值 0~3
* 输    入         : 
* 输    出         :
*******************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);															//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));											//等待转换结束

	return ADC_GetConversionValue(ADC1);																//返回最近一次ADC1规则组的转换结果
}

/*******************************************************************************
* 函 数 名         : u16 Get_Adc_Average(u8 ch,u8 times)
* 函数功能		     : 得到ADC的平均值
* 输    入         : 
* 输    出         :
*******************************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	float temp;
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		temp = temp_val;
		Delay_ms(20);
	}
	return temp_val/times;
} 	 

/*******************************************************************************
* 函 数 名         : DAT_Vol
* 函数功能		     : 传感器采集
* 输    入         : 
* 输    出         : 10进制表示的电压temp_adc
*******************************************************************************/
float DAT_Vol(void)
{
	u16 adcx = 0;
	float temp_adc = 0;
	do{
	  adcx=Get_Adc_Average(ADC_Channel_12,10);
//	LCD12864_wstr(1,0,(unsigned char *)adcx);//显示ADC的值
	  temp_adc=(float)adcx * 2 * (3.0/4096);
  }while(temp_adc > 4.3);
	return temp_adc;
}

/*******************************************************************************
* 函 数 名         : void Adc_Check(void)
* 函数功能		     : 电池电量检测
* 输    入         : 
* 输    出         :
*******************************************************************************/
void Adc_Check(void)
{
  int PO = 0,One = 0;
  unsigned char ele[5];
  //		unsigned char *ele;
  unsigned char Temp_ADC_Vol;
  Temp_ADC_Vol = (unsigned char) (10 *DAT_Vol());     //电压 乘以 10   以一个字节存储

  // 		if(Temp_ADC_Vol > 41 && Temp_ADC_Vol <= 42) 			  //充满电
  // 		{	Battery_Tab = 0x64;
  // 			LCD12864_wstr(1,6,"4.2V");
  // 			LED_RED_Flag=0;
  // 			BEEP_Flag = 0;
  // 		}
  // 		else if(Temp_ADC_Vol > 38 && Temp_ADC_Vol <= 41) 		//电压正常
  // 		{Battery_Tab = 0x63;
  // 		LCD12864_wstr(1,6,"4.0V");
  // 		LED_RED_Flag=0;
  // 		//	LED_RED_Flag=1;
  // 		//	BEEP_Flag = 1;
  // 		}
  // 		else if(Temp_ADC_Vol > 35 && Temp_ADC_Vol <= 38) 		//电压低
  // 		{Battery_Tab = 0x62;
  // 		LCD12864_wstr(1,6,"3.7V");
  // 			LED_RED_Flag=0;
  // 			BEEP_Flag = 0;
  // 		}
  // 		else if(Temp_ADC_Vol <= 35) 											  //严重低电压
  // 		{	Battery_Tab = 0x61;
  // 			LCD12864_wstr(1,6,"3.5V");
  // 			LED_RED_Flag=1;
  // 			BEEP_Flag = 1;
  // 		}

  PO = Temp_ADC_Vol%10;
  One = Temp_ADC_Vol/10;

  ele[0] = One+0x30;
  ele[1] = '.';
  ele[2] = PO+0x30;
  ele[3] = 0x56;
  ele[4] = '\0';
  LCD12864_wstr(1,6,ele);

  if(Temp_ADC_Vol <= 35) 											  //严重低电压
  {	
    Battery_Tab = 0x61;
  //LCD12864_wstr(1,6,"3.5V");
    LED_RED_Flag=1;
    BEEP_Flag = 1;
  }

  if(Battery_Tab==0x61)
  {
    Warn_Type=0x01;
  }
  else Warn_Type=0x00;
}
