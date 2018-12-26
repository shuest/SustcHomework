#ifndef __CRC16_H
#define __CRC16_H




unsigned int MakeCRC16(unsigned char *message, unsigned int len) ;
unsigned char MakeCRC8( unsigned char* dataToCRC,unsigned int leth ) ;
   unsigned char Do_CRC8( unsigned char crc , unsigned char x ) ;
#endif

