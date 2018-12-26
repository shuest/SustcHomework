#ifndef _CRC_H_
#define _CRC_H_

#include "stm32f10x.h"

unsigned char MakeCRC8( unsigned char* dataToCRC,unsigned int leth ); 
unsigned short int MakeCRC16(unsigned char *message, unsigned int len);
#endif
