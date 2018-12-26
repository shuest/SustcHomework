#ifndef __MCP3204_H
#define __MCP3204_H			    
#include "stm32f10x.h"

#define	SPI_MCP3204_CS PAout(1)  //Ñ¡ÖÐMCP3204	
void SPI_MCP3204_Init(void);
u16 SPI_MCP3204_ReadData(u8 channel);
#endif
