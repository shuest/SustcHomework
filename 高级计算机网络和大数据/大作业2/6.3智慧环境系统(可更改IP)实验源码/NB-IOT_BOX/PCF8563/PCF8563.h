#ifndef _PCF8563_H_
#define _PCF8563_H_
#include "I2C.h"

#define ADJUST_RTC_TIME	  0x01
#define GET_RTC_TIME      0x02
#define GET_GSM_TIME      0x03
#define GET_TIME_FOR_RF   0x04
#define GET_RTC_DTIME     0x05
#define GET_RTC_DTIME_GB  0x06
#define ADJUST_RTC_DTIME  0X07
#define ADJUST_RTC_DTIME_GB  0X08

void PCF8563_Init(void);
u8 PCF8563_ReaDAdress(u8 Adress);
void PCF8563_WriteAdress(u8 Adress, u8 DataTX);
void PCF8563_ReadTimes(void);
void PCF8563_CLKOUT_1s(void);
u8 PCF8563_Check(void);
void PCF8563_WriteTimes(unsigned char *buf);
u8 PCF8563_Check(void);
void RTC_OP(unsigned char *buf,unsigned char mode);
void CMOSTimeInit(void);

extern u8 PCF8563_Time[7];
extern u8 RTC_TIME[6];
extern u8 REC_TIME[6];
#endif

