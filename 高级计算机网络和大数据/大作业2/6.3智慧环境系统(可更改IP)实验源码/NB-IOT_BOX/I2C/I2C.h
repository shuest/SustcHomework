#ifndef _I2C_H_
#define _I2C_H_
//#include "sys.h"
#include "IIC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//#define SDA_IN1()  {GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=((u32)8<<28);}
//#define SDA_OUT1() {GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=((u32)3<<28);}

//IO²Ù×÷º¯Êý	 
#define I2C_SCL1    PEout(4) //SCL
#define I2C_SDA1    PEout(5) //SDA	 
#define READ_SDA1   PEin(5)  //ÊäÈëSDA 

void I2C_Init1(void);
void I2C_Start1(void);
void I2C_Stop1(void);
void I2C_WaitAck1(void);
void I2C_WriteByte1(u8 byte);
u8 I2C_ReadByte1(void);
void I2C_Ack1(void);
void I2C_NoAck1(void);
	  
#endif
	

