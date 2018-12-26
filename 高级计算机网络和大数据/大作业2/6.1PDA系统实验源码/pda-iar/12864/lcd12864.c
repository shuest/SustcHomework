#include "LCD12864.h"



/*******************************************************************************
* �� �� ��         : LCD12864_WriteData
* ��������		   : д����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/
void LCD12864_WriteData(unsigned char dat)
{
    unsigned char i;

	LCD12864_RS_CS_H();
	
    for(i=0;i<8;i++)
    {
			if((dat&0x80)==0x80)
				{			
					LCD12864_SDI_H();
				}
			else
				{		
					LCD12864_SDI_L();
				}			
			
			LCD12864_SCLK_L();
			Delay_100us(1);
			LCD12864_SCLK_H();
			Delay_100us(1);
			LCD12864_SCLK_L();
			Delay_100us(1);
			dat<<=1;
    }
}
/*******************************************************************************
* �� �� ��         : LCD12864_WriteCmd
* ��������		   : д����
* ��    ��         : cmd
* ��    ��         : ��
*******************************************************************************/
void LCD12864_WriteCmd(unsigned char cmd)
{
    LCD12864_Busy();
    LCD12864_WriteData(0xf8);    
	  Delay_100us(10);
    LCD12864_WriteData(cmd&0xf0); 
	  Delay_100us(10);
    LCD12864_WriteData((cmd&0x0f)<<4); 
}

/*******************************************************************************
* �� �� ��         : LCD12864_ReadData
* ��������		   : ��ȡ����
* ��    ��         : ��
* ��    ��         : ��ȡ����8λ����
*******************************************************************************/
unsigned char LCD12864_ReadData(void)
{
     unsigned char i,temp1=0,temp2=0;
	
	 	LCD12864_RS_CS_H();  
	
     for(i=0;i<8;i++)   
     {   
           temp1=temp1<<1;   
           LCD12864_SCLK_L();  
		   Delay_100us(1); 
           LCD12864_SCLK_H();  
		   Delay_100us(1);              
           LCD12864_SCLK_L();   
           if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4)) temp1++;   
     }   
     for(i=0;i<8;i++)   
     {   
           temp2=temp2<<1;   
           LCD12864_SCLK_L();  
		   Delay_100us(1);  
           LCD12864_SCLK_H(); 
		   Delay_100us(1);   
           LCD12864_SCLK_L();   
           if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4))  temp2++;   
     }   
	 	LCD12864_RS_CS_L();
     return ((0xf0&temp1)+(0x0f&temp2));   
}
/*******************************************************************************
* �� �� ��         : LCD12864_Busy
* ��������		   : ���LCD�Ƿ�æ
* ��    ��         : ��
* ��    ��         : 1��0��1��ʾ��æ��0��ʾæ��
*******************************************************************************/
void LCD12864_Busy(void)
{
    do
    {
        LCD12864_WriteData(0xfc);
    }
    while((LCD12864_ReadData()&0x8000)==0x8000);
}



void write_char(unsigned char dat)
{
    LCD12864_Busy();
    LCD12864_WriteData(0xfa);//rw=0;rs=1
	Delay_100us(1);
    LCD12864_WriteData(dat&0xf0);
	Delay_100us(1);
    LCD12864_WriteData((dat&0x0f)<<4);
}
/*******************************************************************************
* �� �� ��         : LCD1264_clear
* ��������		     : �����ʾ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LCD1264_clear(void)       
{
    LCD12864_WriteCmd(0x01);
}

/*******************************************************************************
* �� �� ��         : LCD12864_SetWindow
* ��������		     : �����ڻ���ָ��ģʽ��������ʾ���ꡣע�⣺x�������У�y��������
* ��    ��         : x, y
* ��    ��         : ��
*******************************************************************************/
void LCD12864_SetWindow(unsigned char y_add , unsigned char x_add)
{
    switch(y_add)
    {
        case 1:
        LCD12864_WriteCmd(0X80|x_add);break;
        case 2:
        LCD12864_WriteCmd(0X90|x_add);break;
        case 3:
        LCD12864_WriteCmd(0X88|x_add);break;
        case 4:
        LCD12864_WriteCmd(0X98|x_add);break;
        default:break;
    }    
}

/*******************************************************************************
* �� �� ��         : LCD12864_wstr
* ��������		     :���κ�λ��д���֡�ע�⣺x�������У�y��������,str ����
* ��    ��         : x, y,str
* ��    ��         : ��
*******************************************************************************/
void LCD12864_wstr(unsigned char y_add , unsigned char x_add , unsigned char *str)    
{
    unsigned char i;
    LCD12864_SetWindow(y_add , x_add);
    for(i=0;str[i]!='\0';i++)
    {
        write_char(str[i]);  
    }

}
void LCD12864_wstr_Iots(unsigned char y_add , unsigned char x_add ,unsigned char *pbSrc, int nLen)
{
    char	ddl,ddh;
    int i;
    unsigned char pbDest[255];

    for (i=0; i<nLen; i++)
    {
      ddh = 48 + pbSrc[i] / 16;
      ddl = 48 + pbSrc[i] % 16;
      if (ddh > 57) ddh = ddh + 7;
      if (ddl > 57) ddl = ddl + 7;
      
      
      
      pbDest[i*2] = ddh;
      pbDest[i*2+1] = ddl;
    }
    pbDest[nLen*2] = '\0';

    LCD12864_SetWindow(y_add , x_add);
    for(i=0;pbDest[i]!='\0';i++)
    {
        write_char(pbDest[i]);  
    }
}

/*******************************************************************************
* �� �� ��         : LCD12864_Init
* ��������		   : ��ʼ��LCD12864
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LCD12864_Init(void)     
{  
  LCD12864_PW_EN_L();  //��Դ����
  LCD12864_BL_EN_L();
  Delay_ms(100);
  LCD12864_RST_L();
  Delay_100us(10);
  LCD12864_RST_H();

  Delay_100us(100);
  LCD12864_WriteCmd(0x30);
  Delay_100us(1);
  LCD12864_WriteCmd(0x0C);//0000,1100 ������ʾ���α�off���α�λ��off
  Delay_100us(1);
  LCD12864_WriteCmd(0x01);//0000,0001 ��DDRAM
  Delay_100us(1);
//     LCD12864_WriteCmd(0x02);//0000,0010 DDRAM��ַ��λ
//     LCD12864_WriteCmd(0x80);//1000,0000 �趨DDRAM 7λ��ַ000��0000����ַ������A
}

/*****************************************************/
void ID_Display(unsigned char *p)
{
  LCD1264_clear();
  LCD12864_wstr(1,0,"ID");
  LCD12864_wstr_Iots(1,1,p+4,4);
}
// void Hex_To_Dec(unsigned char *p)
// {
// if(*p<0x0A)
// {*p='*p';}
// else
// {
// switch(*p)
// 	{
//    case 0x0A:
// 		 *p='10';
// 	 break;
// 	 case 0x0B:
// 		 *p='11';
// 	 break;
// 	 case 0x0C:
// 		 *p='12';
// 	 break;
// 	 case 0x0D:
// 		 *p='13';
// 	 break;
// 	 case 0x0E:
// 		 *p='14';
// 	 break;
// 	 case 0x0F:
// 		 *p='15';
// 	 break;
// 	 
// 	 default:
// 	 break;
// 	
// 	}
// }
// 	
// }
// /*****************************************************/
// void ID_to_LCD(void)
// {int i;
// unsigned char LED_Local_ID[10];
// 	memcpy(LED_Local_ID,Tab_Local_ID,8);
// 	for(i=0;i<8;i++)
// 	{
// 	Hex_To_Dec(&LED_Local_ID[i]);
// 	}
// 	LCD12864_wstr(1,0,"ID:");
// 	LCD12864_wstr_Iots(1,0,&LED_Local_ID[4],4);
// }