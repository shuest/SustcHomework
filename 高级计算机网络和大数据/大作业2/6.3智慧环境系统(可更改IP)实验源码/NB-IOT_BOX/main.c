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
* 名称: main()
* 功能: 主函数
* 参数: 无
* 返回: 无
* 修改:
* 注释: write by guest
***********************************************************/
void main(void)
{ 
  NB_BoxInit();                                                                   //上电试验箱初始化
  
  for(;;)
  {
    Watchdog();
    DisPlay();
    Watchdog();                                                               //喂狗
    
    GPRSOption();
    
    NBIOTOption();
    
    DeviceJoinNet();

    USART1_ReceiveData();                                                       //检测串口1是否有数据    
    
    LORA_ReceiveData();                                                         //LORA接收数据

    GPS_DataCollection();                                                       //GPS数据接收和处理  

    GPS_DataUpload();                                                            //数据上传

    SwipingCard();                                                              //刷卡上传
    
    WarningUpload();                                                            //报警上传
    
    CycleCheck();                                                               //循环检测处理函数  
  }
}