#include "delay.h"
#include "I2C.h"
#include "PCF8563.h" 
//#include "general.h"

//全局变量,程序初始的值就是要初始化的时间，
//用途：1：初始化时间。2：读取返回时间 
//秒，分，时，天，星期，月份，年份。
u8 PCF8563_Time[7]={00,01,02,03,04,05,16};
u8 Default_time[7] = {0x17,0x06,0x05,0x03,0x02,0x01};//17年6月5日3时2分1秒
u8 RTC_TIME[6]={0};
u8 REC_TIME[6]={0};
u8 test_time[6] = {0};
uint8_t  nowtime[6];

//void BCD2Int(uint8_t *Bcd,uint8_t *Int)
//{
//  *Int = (*Bcd & 0xf0)*10 + (*Bcd & 0x0f);
//}

/*修改时间*/
void PCF8563_Init(void)
{
  //十进制码转换成BCD码
  PCF8563_Time[0] =  ((PCF8563_Time[0]/10) << 4) | (PCF8563_Time[0]%10);
  PCF8563_Time[1] =  ((PCF8563_Time[1]/10) << 4) | (PCF8563_Time[1]%10);
  PCF8563_Time[2] =  ((PCF8563_Time[2]/10) << 4) | (PCF8563_Time[2]%10);
  PCF8563_Time[3] =  ((PCF8563_Time[3]/10) << 4) | (PCF8563_Time[3]%10);
  PCF8563_Time[4] =  ((PCF8563_Time[4]/10 << 4)) | (PCF8563_Time[4]%10);	   //星期不用转换
  PCF8563_Time[5] =  ((PCF8563_Time[5]/10 << 4)) | (PCF8563_Time[5]%10);
  PCF8563_Time[6] =  ((PCF8563_Time[6]/10 << 4)) | (PCF8563_Time[6]%10);
  PCF8563_CLKOUT_1s();
  
  PCF8563_WriteAdress(0x00, 0x20);	//禁止RTC source clock
  //初始化PCF8563的时间
  PCF8563_WriteAdress(0x02, PCF8563_Time[0]);
  PCF8563_WriteAdress(0x03, PCF8563_Time[1]);
  PCF8563_WriteAdress(0x04, PCF8563_Time[2]);
  PCF8563_WriteAdress(0x05, PCF8563_Time[3]);
  PCF8563_WriteAdress(0x06, PCF8563_Time[4]);
  PCF8563_WriteAdress(0x07, PCF8563_Time[5]);
  PCF8563_WriteAdress(0x08, PCF8563_Time[6]);
  PCF8563_WriteAdress(0x00, 0x00);	//Enable RTC sorce clock
}


u8 PCF8563_ReaDAdress(u8 Adress)
{	
  u8 ReadData;

  I2C_Start1();
  I2C_WriteByte1(0xa2); 
  I2C_WaitAck1();
  I2C_WriteByte1(Adress);
  I2C_WaitAck1();
  I2C_Start1();
  I2C_WriteByte1(0xa3);  
  I2C_WaitAck1();
  ReadData = I2C_ReadByte1();
  I2C_Stop1();
  return ReadData;
}

void  PCF8563_WriteAdress(u8 Adress,u8 DataTX)
{
  I2C_Start1();
  I2C_WriteByte1(0xa2);
  I2C_WaitAck1();
  I2C_WriteByte1(Adress);
  I2C_WaitAck1();
  I2C_WriteByte1(DataTX);
  I2C_WaitAck1();
  I2C_Stop1();
}

//取回八个字节的数据:秒，分，时，天，星期，月份，年份。
void PCF8563_ReadTimes(void)
{	
  u8 i = 0,ucAddr = 0x02;
  for(i = 0;i < 7;i++)
  {
    PCF8563_Time[i] = PCF8563_ReaDAdress(ucAddr);
    ucAddr++;
  }
  PCF8563_Time[0] &= 0x7f;
  PCF8563_Time[1] &= 0x7f;
  PCF8563_Time[2] &= 0x3f;
  PCF8563_Time[3] &= 0x3f;
  PCF8563_Time[4] &= 0x07;
  PCF8563_Time[5] &= 0x1f;
//  I2C_Start1();
//  I2C_WriteByte1(0xa2);
//  I2C_WaitAck1();
//  I2C_WriteByte1(0x02);
//  I2C_WaitAck1();
//  I2C_Start1();
//  I2C_WriteByte1(0xa3);
//  I2C_WaitAck1();
//  PCF8563_Time[0] = I2C_ReadByte1()&0x7f;
//  I2C_Ack1();
//  
//  PCF8563_Time[1] = I2C_ReadByte1()&0x7f;
//  I2C_Ack1();
//
//  PCF8563_Time[2] = I2C_ReadByte1()&0x3f;
//  I2C_Ack1();
//
//  PCF8563_Time[3] = I2C_ReadByte1()&0x3f;
//  I2C_Ack1();
//  
//  PCF8563_Time[4] = I2C_ReadByte1()&0x07;
//  I2C_Ack1();
//  
//  PCF8563_Time[5] = I2C_ReadByte1()&0x1f;
//  I2C_Ack1();
//  
//  PCF8563_Time[6] = I2C_ReadByte1();
//  I2C_NoAck1();
//  I2C_Stop1();

//  PCF8563_Time[0] = ((PCF8563_Time[0]&0xf0)>>4)*10 + (PCF8563_Time[0]&0x0f);
//  PCF8563_Time[1] = ((PCF8563_Time[1]&0xf0)>>4)*10 + (PCF8563_Time[1]&0x0f);
//  PCF8563_Time[2] = ((PCF8563_Time[2]&0xf0)>>4)*10 + (PCF8563_Time[2]&0x0f);
//  PCF8563_Time[3] = ((PCF8563_Time[3]&0xf0)>>4)*10 + (PCF8563_Time[3]&0x0f);
//  PCF8563_Time[4] = ((PCF8563_Time[4]&0xf0)>>4)*10 + (PCF8563_Time[4]&0x0f);
//  PCF8563_Time[5] = ((PCF8563_Time[5]&0xf0)>>4)*10 + (PCF8563_Time[5]&0x0f);
//  PCF8563_Time[6] = ((PCF8563_Time[6]&0xf0)>>4)*10 + (PCF8563_Time[6]&0x0f);

//八个字节的数据:秒，分，时，天，星期，月份，年份。
  RTC_TIME[0]=PCF8563_Time[6];
  RTC_TIME[1]=PCF8563_Time[5];
  RTC_TIME[2]=PCF8563_Time[3];
  RTC_TIME[3]=PCF8563_Time[2];
  RTC_TIME[4]=PCF8563_Time[1];
  RTC_TIME[5]=PCF8563_Time[0];
  
 // while(PCF8563_Check());

//  REC_TIME[0]=((PCF8563_Time[6]/10)<<4)+(PCF8563_Time[6]%10);
//  REC_TIME[1]=((PCF8563_Time[5]/10)<<4)+(PCF8563_Time[5]%10);
//  REC_TIME[2]=((PCF8563_Time[3]/10)<<4)+(PCF8563_Time[3]%10);
//  REC_TIME[3]=((PCF8563_Time[2]/10)<<4)+(PCF8563_Time[2]%10);
//  REC_TIME[4]=((PCF8563_Time[1]/10)<<4)+(PCF8563_Time[1]%10);
//  REC_TIME[5]=((PCF8563_Time[0]/10)<<4)+(PCF8563_Time[0]%10);  
}

//void PCF8563_WriteTimes(unsigned char *buf)
//{
//  I2C_Init1();
//  delay_ms(100);
//
//
//  PCF8563_CLKOUT_1s();
//  //禁止RTC source clock
//  PCF8563_WriteAdress(0x00, 0x20);
//            
//  //重新写入设置时间到PCF8563
//  PCF8563_WriteAdress(0x02, buf[5]);
//  PCF8563_WriteAdress(0x03, buf[4]);
//  PCF8563_WriteAdress(0x04, buf[3]);
//  PCF8563_WriteAdress(0x05, buf[2]);
//  PCF8563_WriteAdress(0x06, buf[6]);
//  PCF8563_WriteAdress(0x07, buf[1]);
//  PCF8563_WriteAdress(0x08, buf[0]);
//  //Enable RTC sorce clock
//  PCF8563_WriteAdress(0x00, 0x00);	
//} 

//在CLKOUT上定时1S输出一个下降沿脉冲，经过验证，可以设置STM32的GPIO上拉输入，设置成下降沿中断，单片机每过1S产生一次中断
void PCF8563_CLKOUT_1s(void)
{
    PCF8563_WriteAdress(0x01, 0);	//禁止定时器输出，闹铃输出
    PCF8563_WriteAdress(0x0e, 0);	//关闭定时器等等
    PCF8563_WriteAdress(0x0d, 0x83);  //打开输出脉冲
}

u8 PCF8563_Check(void)
{
  //  I2C_Init1();
  //  PCF8563_ReadTimes();
    if((RTC_TIME[0]>0x20)||(RTC_TIME[0]<0x13))	 //年
    return 1;
    else if((RTC_TIME[1]>0x12)||(RTC_TIME[1]<0x01))  //月
    return 1;
    else if((RTC_TIME[2]>0x31)||(RTC_TIME[2]<0x01))  //日
    return 1;
    else if(RTC_TIME[3]>0x023)			 //时
    return 1;
    else if(RTC_TIME[4]>0x059)			 //分
    return 1;
    else if(RTC_TIME[5]>0x059)			 //秒
    return 1;
    else
    return 0;
}

void RTC_OP(unsigned char *buf,unsigned char mode)
{	
//  uint8_t i = 0;
  switch(mode)
  {
      case  ADJUST_RTC_TIME:
      PCF8563_Time[6]=(buf[0]>>4)*10+(buf[0]&0x0f);
      PCF8563_Time[5]=(buf[1]>>4)*10+(buf[1]&0x0f);
      PCF8563_Time[3]=(buf[2]>>4)*10+(buf[2]&0x0f);
      PCF8563_Time[2]=(buf[3]>>4)*10+(buf[3]&0x0f);
      PCF8563_Time[1]=(buf[4]>>4)*10+(buf[4]&0x0f);
      PCF8563_Time[0]=(buf[5]>>4)*10+(buf[5]&0x0f);
//      for(i = 0;i < 6;i++)
//      {
//        BCD2Int(&buf[i],&buf[i]);
//      }
//      PCF8563_Time[6]=buf[0];
//      PCF8563_Time[5]=buf[1];
//      PCF8563_Time[3]=buf[2];
//      PCF8563_Time[2]=buf[3];
//      PCF8563_Time[1]=buf[4];
//      PCF8563_Time[0]=buf[5];
      PCF8563_Init();

      break;
      
      case GET_RTC_TIME:
      PCF8563_ReadTimes();
      buf[0]=RTC_TIME[0];
      buf[1]=RTC_TIME[1];
      buf[2]=RTC_TIME[2];
      buf[3]=RTC_TIME[3];
      buf[4]=RTC_TIME[4];
      buf[5]=RTC_TIME[5];
      break;
      
//      case GET_RTC_TIME:
//      PCF8563_ReadTimes();
//      buf[0]=REC_TIME[0];
//      buf[1]=REC_TIME[1];
//      buf[2]=REC_TIME[2];
//      buf[3]=REC_TIME[3];
//      buf[4]=REC_TIME[4];
//      buf[5]=REC_TIME[5];
//      break;
      
//     case ADJUST_RTC_DTIME_GB:
//      if(buf[7]!=Do_XOR(buf,7))
//      {break;}
//      PCF8563_WriteTimes(buf);
//      break;      
//      
//      case GET_RTC_DTIME_GB:
//      PCF8563_ReadTimes();
//      {
//        buf[0]=0x20;
//        buf[1]=(RTC_TIME[0]/10*16)+(RTC_TIME[0]%10);
//        buf[2]=(RTC_TIME[1]/10*16)+(RTC_TIME[1]%10);
//        buf[3]=(RTC_TIME[2]/10*16)+(RTC_TIME[2]%10);
//        buf[4]=(RTC_TIME[3]/10*16)+(RTC_TIME[3]%10);
//        buf[5]=(RTC_TIME[4]/10*16)+(RTC_TIME[4]%10);
//        buf[6]=(RTC_TIME[5]/10*16)+(RTC_TIME[5]%10);
//      }
//      buf[7]=Do_XOR(buf,7);
//      break;      
      
      default:break;
  }
} 

void CMOSTimeInit(void)
{
//  uint8_t time[7] = {0};
  RTC_OP(Default_time,ADJUST_RTC_TIME);
 // PCF8563_Init();
//  while(1)
//    RTC_OP(time,GET_RTC_TIME);
  
}


