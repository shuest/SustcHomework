#include "GPSLib.h"
#include "gps.h"

extern uint8_t SetGPRSModeSuccessFlag;
extern uint8_t SetGPSModeTim;
extern uint8_t GPS_WorkMode;                     ///GPS工作模式
extern uint8_t GPSMode[31];
extern uint8_t BDMode[31];
extern uint8_t BothMode[31];
extern uint8_t GPS_UartDataFinishFlag;
extern uint8_t CP_GPSData(uint8_t *string);
extern uint8_t GPSNotRemsg;
extern uint8_t GPS_RxBuf[GPS_MAX_len];

uint8_t SetGPSCount = 0;
uint8_t SetGPSModeNum = 0;                                              //失败次数

void GPS_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  
    
    /* 配置GPS POWER为推挽输出模式 RED*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);   
}

void GPS_Init(void)
{
  GPS_GPIOConfig();
  GPS_uart5_init();
  GPS_Start();
}

void FrashBufGPS(uint8_t *buf)
{
  int i = 0;
  for(i = 0;i < 1020;i++)
  {
    *buf = 0;
    buf++;
  }
}

void SetGPSMode(void)
{
  if(SetGPSModeTim >= 1 && !SetGPRSModeSuccessFlag)
  {
    SetGPSModeTim = 0;
    if(!SetGPSCount)
    {
      SetGPSCount = 1;
      SetGPSModeNum = 0;
      FrashBufGPS(GPS_RxBuf);                                     //清空接收缓冲区
      switch(GPS_WorkMode)                                      //设置GPS工作模式
      {
        case 0x01:                                              //GPS
        {
          Usart_SendNumString9600(UART5,GPSMode,31);
          break;
        }
        case 0x02:                                              //北斗
        {
          Usart_SendNumString9600(UART5,BDMode,31);
          break;
        }
        case 0x03:                                              //混合
        {
          Usart_SendNumString9600(UART5,BothMode,31);
          break;
        }
        default:
          break;
      }
    }
    if(GPS_UartDataFinishFlag)
    {
      GPS_UartDataFinishFlag = 0;
      USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);                               //关中断
      if((CP_GPSData("$PMTK011") == 1) || (CP_GPSData("$PMTK001") == 1))
      {
        SetGPRSModeSuccessFlag = 1;
        SetGPSModeNum = 0;
        GPS_Start();
      }
      USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);                               //开中断
    }

    SetGPSModeNum++;
    if(SetGPSModeNum >= 20)                                                       //设置失败  重发
    {
      SetGPSModeNum = 0;
      if(!SetGPRSModeSuccessFlag)
      {
        SetGPSCount = 0;
        GPSNotRemsg++;
        if(GPSNotRemsg >= 2)                                                      //3次重启GPS
        {
          GPSNotRemsg = 0;
          if(GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4))
            GPS_OFF();
          else
            GPS_Start();
        }
      }
    }
  }
}

void GPS_DataCollection(void)
{
  GPS_ReceiveData();                                                            //GPS接收数据

//  SetGPSMode();                                                                 //设置GPS工作模式
//  
//  if(SetGPRSModeSuccessFlag)
//  {
    GPS_DataCheck();                                                            //GPS数据提取与解析
    USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);                               //开中断
//  }
}