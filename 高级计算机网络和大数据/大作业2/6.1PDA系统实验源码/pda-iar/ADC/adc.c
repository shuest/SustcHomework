 #include "adc.h"
//#include "delay.h"
		   		
/*******************************************************************************
* �� �� ��         : void  Adc_Init(void) 
* ��������		     : ��ʼ��ADC��ch:ͨ��Ĭ��ֵ 0~3
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   						//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
                        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	ADC_DeInit(ADC1);  															//��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 12;														//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);																	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);											//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);									//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);								  //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 		//�ȴ�У׼����
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}				  

/*******************************************************************************
* �� �� ��         : u16 Get_Adc(u8 ch) 
* ��������		     : ���ADCֵ��ch:ͨ��ֵ 0~3
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
u16 Get_Adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);															//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));											//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);																//�������һ��ADC1�������ת�����
}

/*******************************************************************************
* �� �� ��         : u16 Get_Adc_Average(u8 ch,u8 times)
* ��������		     : �õ�ADC��ƽ��ֵ
* ��    ��         : 
* ��    ��         :
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
* �� �� ��         : DAT_Vol
* ��������		     : �������ɼ�
* ��    ��         : 
* ��    ��         : 10���Ʊ�ʾ�ĵ�ѹtemp_adc
*******************************************************************************/
float DAT_Vol(void)
{
	u16 adcx = 0;
	float temp_adc = 0;
	do{
	  adcx=Get_Adc_Average(ADC_Channel_12,10);
//	LCD12864_wstr(1,0,(unsigned char *)adcx);//��ʾADC��ֵ
	  temp_adc=(float)adcx * 2 * (3.0/4096);
  }while(temp_adc > 4.3);
	return temp_adc;
}

/*******************************************************************************
* �� �� ��         : void Adc_Check(void)
* ��������		     : ��ص������
* ��    ��         : 
* ��    ��         :
*******************************************************************************/
void Adc_Check(void)
{
  int PO = 0,One = 0;
  unsigned char ele[5];
  //		unsigned char *ele;
  unsigned char Temp_ADC_Vol;
  Temp_ADC_Vol = (unsigned char) (10 *DAT_Vol());     //��ѹ ���� 10   ��һ���ֽڴ洢

  // 		if(Temp_ADC_Vol > 41 && Temp_ADC_Vol <= 42) 			  //������
  // 		{	Battery_Tab = 0x64;
  // 			LCD12864_wstr(1,6,"4.2V");
  // 			LED_RED_Flag=0;
  // 			BEEP_Flag = 0;
  // 		}
  // 		else if(Temp_ADC_Vol > 38 && Temp_ADC_Vol <= 41) 		//��ѹ����
  // 		{Battery_Tab = 0x63;
  // 		LCD12864_wstr(1,6,"4.0V");
  // 		LED_RED_Flag=0;
  // 		//	LED_RED_Flag=1;
  // 		//	BEEP_Flag = 1;
  // 		}
  // 		else if(Temp_ADC_Vol > 35 && Temp_ADC_Vol <= 38) 		//��ѹ��
  // 		{Battery_Tab = 0x62;
  // 		LCD12864_wstr(1,6,"3.7V");
  // 			LED_RED_Flag=0;
  // 			BEEP_Flag = 0;
  // 		}
  // 		else if(Temp_ADC_Vol <= 35) 											  //���ص͵�ѹ
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

  if(Temp_ADC_Vol <= 35) 											  //���ص͵�ѹ
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
