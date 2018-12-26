#ifndef __ADC_H
#define __ADC_H	
#include "stm32f10x.h"

#define Elec_VLow 0x61
#define Elec_Low 0x62
#define Elec_Normal 0x63
#define Elec_Full 0x64

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
float DAT_Vol(void);
#endif 
