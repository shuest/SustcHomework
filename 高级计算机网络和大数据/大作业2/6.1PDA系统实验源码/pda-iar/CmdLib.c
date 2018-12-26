#include "CmdLib.h"
#include "Command.h"

extern uint8_t ReSendFlag;
extern unsigned char Sensor_Package_Send2[50];
extern Um402_Command_TYPE   Um402_Temp_Data_Update2;         //结构定义 

extern uint8_t KeyTimCount;
extern uint8_t Key_Count;

uint8_t ReSendCount = 0;                //重传计数

/*******************************************************************************
* 函 数 名         : 串口数据转存
* 函数功能		     : 
* 输    入         : UART1_Rcvbuf
* 输    出         : UART_Data_1，UART_Data_2，UART_Data_3，UART_Data_4，UART_Data_5
*******************************************************************************/
void  Data_UART(void)  
{
    unsigned char i,m,*p;

    unsigned int  CRC16,CRC16_2;

    for(i=0;i<UART1_pRcv;i++)
    {
      if(UART1_Rcvbuf[i]==0x7B)
      {
        CRC16=UART1_Rcvbuf[UART1_Rcvbuf[i+11]+i+12];			              		
        CRC16=(CRC16<<8)+(UART1_Rcvbuf[UART1_Rcvbuf[i+11]+i+13]);          //数据包中的CRC校验码
        CRC16_2=MakeCRC16(&UART1_Rcvbuf[i],(UART1_Rcvbuf[i+11]+12));			 //根据数据包中的数据计算出来的CRC
        if(CRC16==CRC16_2)
        {
          if((UART_Flage_Command&0x01)==0)                               //判断UART_Data_1中是否有转存的数据包                          
          {
            p=(unsigned char *)&UART_Data_1;
            UART_Flage_Command=UART_Flage_Command|0x01;	
          }
          else if((UART_Flage_Command&0x02)==0)													 //判断UART_Data_2中是否有转存的数据包   
          {
            p=(unsigned char *)&UART_Data_2;
            UART_Flage_Command=UART_Flage_Command|0x02;						
          }
          for(m=0;m<(UART1_Rcvbuf[i+11]+12);m++)                     //将UART1_Rcvbuf中的数据传到UART_Data_1或UART_Data_2中
          {
            *p=UART1_Rcvbuf[i+m];
             p++;
          }
        }
      }
    }
    UART1_pRcv=0;		
}

/*******************************************************************************
* 函 数 名         : LORA串口数据转存
* 函数功能		     : 
* 输    入         : UART4_Rcvbuf
* 输    出         : Um402_Data_1，Um402_Data_2，Um402_Data_3，Um402_Data_4，Um402_Data_5
*******************************************************************************/
void  Data_Um402(void)
{
  unsigned char i,m,*p;

  unsigned int  CRC16,CRC16_2;

  UART1_Put(UART4_Rcvbuf,UART4_pRcv);//DEBUGGGGGGGGG

  for(i=0;i<UART4_pRcv;i++)
  {
      if(UART4_Rcvbuf[i]==0x7B)
      {
        CRC16=UART4_Rcvbuf[UART4_Rcvbuf[i+19]+i+20];
        
        CRC16=(CRC16<<8)+(UART4_Rcvbuf[(UART4_Rcvbuf[i+19])+i+21]);    //数据包中的CRC校验码
        
        CRC16_2=MakeCRC16(&UART4_Rcvbuf[i],(UART4_Rcvbuf[i+19]+20));   //根据数据包中的数据计算出来的CRC

        if(CRC16==CRC16_2)
        {							
          if((UM402_Flage_Command&0x01)!=0x01) 											 //判断Um402_Data_1中是否有转存的数据包  
          {
            p=(unsigned char *)&Um402_Data_1;
            UM402_Flage_Command=UM402_Flage_Command|0x01;	
          }
          else if((UM402_Flage_Command&0x02)!=0x02)							   //判断Um402_Data_2中是否有转存的数据包   
          {
            p=(unsigned char *)&Um402_Data_2;
            UM402_Flage_Command=UM402_Flage_Command|0x02;
          }

          for(m=0;m<(UART4_Rcvbuf[i+19]+22);m++)								 //将UART4_Rcvbuf中的数据传到Um402_Data_1或Um402_Data_2中
          {
            *p=UART4_Rcvbuf[i+m];
             p++;
          }
        }
      }
      else if(UART4_Rcvbuf[i]==0x24)
      {
        LORA_SETA_L();
        LORA_SETB_L();
      }
    }
    UART4_pRcv=0;		
}

/*******************************************************************************
* 函 数 名         :void Timing_Process(void)
* 函数功能		     :定时处理任务
* 输    入         : 
* 输    出         : 
*******************************************************************************/
void Timing_Process(void)
{
  if (( time4 > (Sensor_Time) )&&(Sensor_Mode==0x01)) 
  {			    
    Sensor_Data_Update();					//传感器数据采集  与协议封装	
    Lcd_Tick=0;
    Lcd_Ready_Flag = 1;
    LCD12864_wstr(2,0,"传感信息已上传  ");
    LCD12864_wstr(3,0,"                ");	
    LCD12864_wstr(4,0,"                ");	
    Watchdog();									  //看门狗喂狗				
    time4=0;		
  } 			
  if ( Adc_time > 0x12 ) 						//电池电量检测
  {			    
    Adc_Check();
  } 	
  
  Watchdog();		
  if(LED_BLUE_Flag==1)
          LED_BLUE_H();
  else
          LED_BLUE_L();
  
  if((LED_RED_TIME_OK!=0)&&(LED_RED_Flag==1))
          LED_RED_H();
  else
          LED_RED_L();
  

  if((BEEP_TIME_OK!=0)&&(BEEP_Flag == 1))		//蜂鸣器			
          BEEP_H();
  else
          BEEP_L();
  
  if(Lcd_Flag == 1) 													//液晶屏中间两行刷新	
  {
    Lcd_Flag = 0;
    LCD12864_wstr(2,0,"                ");	
    LCD12864_wstr(3,0,"                ");
  }
  Watchdog();				
}

/*******************************************************************************
* 函 数 名         : void Check_Key(void)
* 函数功能		     : 按键功能处理
* 输    入         : 
* 输    出         : 
*******************************************************************************/
void Check_Key(void)
{
  if(Key_Flag==1)
  {
      Key_Tick=0;
      Key_Val=KEY_Scan(1);
      if(Key_Val!=0)
         Key_To_Um402();                                                        //按键发送
  }
  
  if(KeyTimCount >= 5 && Key_Count)                                               //5S不操作
  {
    KeyTimCount = 0;
    Key_Count = 0;
  }
  
  if(time3 >= 3000 && ReSendFlag)                                       //重发
  {
    time3 = 0;
    UART4_Put(Sensor_Package_Send2,((Um402_Temp_Data_Update2.Instruction_Length)+22));          //重发内容 4次
    ReSendCount++;
    if(ReSendCount > 3)
    {
      ReSendCount = 0;
      ReSendFlag = 0;
    }
  }
  Watchdog();
}