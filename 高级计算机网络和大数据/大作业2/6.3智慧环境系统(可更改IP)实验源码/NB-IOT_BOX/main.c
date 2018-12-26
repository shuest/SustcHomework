#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "usart.h"
#include "NBIOTLib.h"
#include "gpio.h"
#include "lock.h"
#include "24cxx.h" 
#include "LCD12864.h"
#include "adc.h"
#include "gps.h"
#include "GPSLib.h"
#include "GPRSLib.h"
#include "UM402Lib.h"

uint8_t NBIOT = 0;
uint8_t GPRS = 0;
/***********************************************************
* ����: main()
* ����: ������
* ����: ��
* ����: ��
* �޸�:
* ע��: write by guest
***********************************************************/
void main(void)
{ 
  NB_BoxInit();                                                                   //�ϵ��������ʼ��
  
  for(;;)
  {
    Watchdog();
    DisPlay();
    Watchdog();                                                               //ι��
    
    GPRSOption();
    
    NBIOTOption();
    
    DeviceJoinNet();

    USART1_ReceiveData();                                                       //��⴮��1�Ƿ�������    
    
    LORA_ReceiveData();                                                         //LORA��������

    GPS_DataCollection();                                                       //GPS���ݽ��պʹ���  

    GPS_DataUpload();                                                            //�����ϴ�

    SwipingCard();                                                              //ˢ���ϴ�
    
    WarningUpload();                                                            //�����ϴ�
    
    CycleCheck();                                                               //ѭ����⴦����  
  }
}