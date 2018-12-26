#ifndef _GPS_H_
#define _GPS_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "usart.h"

#define GPS_MAX_len 1024
#define GPS_Data_Len 1024

#define GPS_BD 1                //±±¶·
#define GPS_GP 2                //GPS
#define GPS_GN 3                //GNSS


#define GPS_Start() GPIO_SetBits(GPIOD,GPIO_Pin_4)

#define GPS_OFF() GPIO_ResetBits(GPIOD,GPIO_Pin_4)

void GPS_DataUpload(void);
void GPS_DataCheck(void);
void GPS_Init(void);
uint8_t ASCII2HEX(uint8_t ASICC);
void GLWZ2BJtime(uint8_t  *Time_buffer);
void GpsTime2S(uint8_t *tBuf,uint8_t *src);
uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cx);
uint32_t NMEA_Pow(uint8_t m,uint8_t n);
uint32_t NMEA_Str2num(uint8_t *buf,uint8_t *dx);
void LatChange(uint8_t *buf,uint32_t lat);
void SetGPSMode(void);

#endif
