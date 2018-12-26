#include "CmdLib.h"
#include "Command.h"

extern uint8_t ReSendFlag;
extern unsigned char Sensor_Package_Send2[50];
extern Um402_Command_TYPE   Um402_Temp_Data_Update2;         //�ṹ���� 

extern uint8_t KeyTimCount;
extern uint8_t Key_Count;

uint8_t ReSendCount = 0;                //�ش�����

/*******************************************************************************
* �� �� ��         : ��������ת��
* ��������		     : 
* ��    ��         : UART1_Rcvbuf
* ��    ��         : UART_Data_1��UART_Data_2��UART_Data_3��UART_Data_4��UART_Data_5
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
        CRC16=(CRC16<<8)+(UART1_Rcvbuf[UART1_Rcvbuf[i+11]+i+13]);          //���ݰ��е�CRCУ����
        CRC16_2=MakeCRC16(&UART1_Rcvbuf[i],(UART1_Rcvbuf[i+11]+12));			 //�������ݰ��е����ݼ��������CRC
        if(CRC16==CRC16_2)
        {
          if((UART_Flage_Command&0x01)==0)                               //�ж�UART_Data_1���Ƿ���ת������ݰ�                          
          {
            p=(unsigned char *)&UART_Data_1;
            UART_Flage_Command=UART_Flage_Command|0x01;	
          }
          else if((UART_Flage_Command&0x02)==0)													 //�ж�UART_Data_2���Ƿ���ת������ݰ�   
          {
            p=(unsigned char *)&UART_Data_2;
            UART_Flage_Command=UART_Flage_Command|0x02;						
          }
          for(m=0;m<(UART1_Rcvbuf[i+11]+12);m++)                     //��UART1_Rcvbuf�е����ݴ���UART_Data_1��UART_Data_2��
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
* �� �� ��         : LORA��������ת��
* ��������		     : 
* ��    ��         : UART4_Rcvbuf
* ��    ��         : Um402_Data_1��Um402_Data_2��Um402_Data_3��Um402_Data_4��Um402_Data_5
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
        
        CRC16=(CRC16<<8)+(UART4_Rcvbuf[(UART4_Rcvbuf[i+19])+i+21]);    //���ݰ��е�CRCУ����
        
        CRC16_2=MakeCRC16(&UART4_Rcvbuf[i],(UART4_Rcvbuf[i+19]+20));   //�������ݰ��е����ݼ��������CRC

        if(CRC16==CRC16_2)
        {							
          if((UM402_Flage_Command&0x01)!=0x01) 											 //�ж�Um402_Data_1���Ƿ���ת������ݰ�  
          {
            p=(unsigned char *)&Um402_Data_1;
            UM402_Flage_Command=UM402_Flage_Command|0x01;	
          }
          else if((UM402_Flage_Command&0x02)!=0x02)							   //�ж�Um402_Data_2���Ƿ���ת������ݰ�   
          {
            p=(unsigned char *)&Um402_Data_2;
            UM402_Flage_Command=UM402_Flage_Command|0x02;
          }

          for(m=0;m<(UART4_Rcvbuf[i+19]+22);m++)								 //��UART4_Rcvbuf�е����ݴ���Um402_Data_1��Um402_Data_2��
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
* �� �� ��         :void Timing_Process(void)
* ��������		     :��ʱ��������
* ��    ��         : 
* ��    ��         : 
*******************************************************************************/
void Timing_Process(void)
{
  if (( time4 > (Sensor_Time) )&&(Sensor_Mode==0x01)) 
  {			    
    Sensor_Data_Update();					//���������ݲɼ�  ��Э���װ	
    Lcd_Tick=0;
    Lcd_Ready_Flag = 1;
    LCD12864_wstr(2,0,"������Ϣ���ϴ�  ");
    LCD12864_wstr(3,0,"                ");	
    LCD12864_wstr(4,0,"                ");	
    Watchdog();									  //���Ź�ι��				
    time4=0;		
  } 			
  if ( Adc_time > 0x12 ) 						//��ص������
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
  

  if((BEEP_TIME_OK!=0)&&(BEEP_Flag == 1))		//������			
          BEEP_H();
  else
          BEEP_L();
  
  if(Lcd_Flag == 1) 													//Һ�����м�����ˢ��	
  {
    Lcd_Flag = 0;
    LCD12864_wstr(2,0,"                ");	
    LCD12864_wstr(3,0,"                ");
  }
  Watchdog();				
}

/*******************************************************************************
* �� �� ��         : void Check_Key(void)
* ��������		     : �������ܴ���
* ��    ��         : 
* ��    ��         : 
*******************************************************************************/
void Check_Key(void)
{
  if(Key_Flag==1)
  {
      Key_Tick=0;
      Key_Val=KEY_Scan(1);
      if(Key_Val!=0)
         Key_To_Um402();                                                        //��������
  }
  
  if(KeyTimCount >= 5 && Key_Count)                                               //5S������
  {
    KeyTimCount = 0;
    Key_Count = 0;
  }
  
  if(time3 >= 3000 && ReSendFlag)                                       //�ط�
  {
    time3 = 0;
    UART4_Put(Sensor_Package_Send2,((Um402_Temp_Data_Update2.Instruction_Length)+22));          //�ط����� 4��
    ReSendCount++;
    if(ReSendCount > 3)
    {
      ReSendCount = 0;
      ReSendFlag = 0;
    }
  }
  Watchdog();
}