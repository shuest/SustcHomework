#ifndef __LCD12864_H
#define __LCD12864_H

#include "stm32f10x.h"


void LCD12864_WriteData(unsigned char dat);
void LCD12864_WriteCmd(unsigned char cmd);
unsigned char LCD12864_ReadData(void);
void LCD12864_Busy(void);
void write_char(unsigned char dat);
void LCD1264_clear(void);        //«Â≥˝œ‘ æ


void LCD12864_SetWindow(unsigned char y_add , unsigned char x_add);
void LCD12864_wstr(unsigned char y_add , unsigned char x_add , unsigned char *str);
void LCD12864_figer(unsigned char y_add , unsigned char x_add , unsigned int figer);

void LCD12864_Init(void);

void LCD12864_wstr_Iots(unsigned char y_add , unsigned char x_add ,unsigned char *pbSrc, int nLen);
void ID_Display(unsigned char *p);
// void Hex_To_Dec(unsigned char *p);
// void ID_to_LCD(void);
#endif
