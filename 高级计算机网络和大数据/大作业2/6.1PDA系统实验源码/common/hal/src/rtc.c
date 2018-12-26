
#include "rtc.h"
#include <stdio.h>
#include "misc.h"
#include "stm32f10x.h"
/***********************************************************
* ����: rtc_init()
* ����: ��ʼ��rtc�� ������rtc�ж�
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void rtc_init(void)
{
  //�������ṩʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
  // ʹ��д Backup domain 
  PWR_BackupAccessCmd(ENABLE);
  //���ⲿ��Ƶ����
  RCC_LSEConfig(RCC_LSE_ON);
  //�ȴ��ⲿ��Ƶ����������
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
  //�ⲿ��Ƶ������ΪRTC����Դ
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  //ʹ��RTC
  RCC_RTCCLKCmd(ENABLE);

  RTC_WaitForSynchro();

  RTC_WaitForLastTask();
  // ʹ��RTC���ж�
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  //�ȴ�
  RTC_WaitForLastTask(); 
  //(32.768 KHz)/(32767+1)
  RTC_SetPrescaler(32767);

  RTC_WaitForLastTask();
   
  //���� rtc �ж�
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/***********************************************************
* ����: set_rtc_time()
* ����: ����rtc ʱ�ӵĳ�ʼ��ֵ
* ����: ���� : char year :�趨�ꣻ
*       ���� : moth���趨��11��
*       ���� : char day ���趨���ڣ�
*       ���� : hour���趨Сʱ��
*       ���� : char min ���趨���ӣ�
*       ���� : char sec ���趨�룻
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void set_rtc_time(unsigned char year ,unsigned char moth,
                unsigned char day ,unsigned char hour,
                unsigned char min ,unsigned char sec)
{
  unsigned long temp;
  temp = year*360*24*3600+(moth-1)*30*24*3600+(day-1)*24*3600+hour*3600+min*60+sec;
  RTC_WaitForLastTask();

  RTC_SetCounter(temp);

  RTC_WaitForLastTask(); 
}
/***********************************************************
* ����: read_rtc_time()
* ����: ��ȡrtc ʱ�ӵ�ֵ�������ڴ����д�ӡ������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/

void read_rtc_time(void)
{
  unsigned long Time_Value;
  unsigned short Day_Value;
  Time_Struct TimeStruct1;
     
  Time_Value = RTC_GetCounter();
  Day_Value = Time_Value/(24*3600) ;
         
  TimeStruct1.Year = Day_Value/360;
  TimeStruct1.Month = (Day_Value - TimeStruct1.Year*360)/30;
  TimeStruct1.Day = (Day_Value - TimeStruct1.Year*360)%30;
  TimeStruct1.Month += 1;
  TimeStruct1.Day += 1;
          
  TimeStruct1.Hour =  (Time_Value - Day_Value *24*3600)/3600;         
  TimeStruct1.Min =   (Time_Value - Day_Value *24*3600 - TimeStruct1.Hour*3600)/60;
  TimeStruct1.Sec =    Time_Value - Day_Value *24*3600 - TimeStruct1.Hour*3600 - TimeStruct1.Min*60;
         
  printf("20%d��  %d��  %d��\r\n", TimeStruct1.Year, TimeStruct1.Month, TimeStruct1.Day);
  printf("%dh : %dm : %ds\n", TimeStruct1.Hour, TimeStruct1.Min, TimeStruct1.Sec );           
}
/***********************************************************
* ����: RTC_IRQHandler()
* ����: RTC�жϴ�������ÿ1�����һ���жϣ� ÿ���жϽ�ʼ�մ�ӡ��������
* ����: ��
* ����: ��
* �޸�:
* ע��: 
***********************************************************/
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_SEC)!=RESET)
   {
     RTC_ClearFlag(RTC_IT_SEC);
     read_rtc_time();
   }
}