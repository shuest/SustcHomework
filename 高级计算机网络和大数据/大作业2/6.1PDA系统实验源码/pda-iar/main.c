#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "CmdLib.h"

#define  EEP_Firstpage      0x00 					  //EEPROM 起始地址 
unsigned  int Self_Test = 0xFFFF;           //上电自检标志  EEPROM 是否修改标志位存储位置地址（2位）
/*
   0x00  EEPROM 起始地址
	 EEPROM 是否修改标志位存储位置地址（2位）：0x01-0x02
	 设备ID存储地址（8位）									 ：0x11-0x18
	 操作密码 存储位置（10位）               ：0x21-0X2A
	 传感器上传时间（2位）                   ：0x2D
	 
	 LORA配置信息存储 （23位）               ：0x30-0x46
	 GPS上传时间（2位）                      ：0x50
	 定位芯片工作模式                        ：0x=58;    01GPS，02BD   03双模
	 温湿度采集模式 1byte                    : 0x5C;	    01自动，02手动
*/

/*******************************************************************************
* 函 数 名         : mian
* 函数功能		     : 主函数
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/
int main()
{ 
    PDA_Init();    			   //PDA初始化          

    Self_Check();					 //检测整机参数是否配置
  
    Param_Get();           //若整机参数已经配置，将E2PROM中的数据读入全局变量

    Adc_Check();           //电池电量检测
    while(1)
    {	 		
        Watchdog();						 //看门狗喂狗
        Check_Uart1();   	   //检测串口1是否有数据  

        Check_Uart4();       //检测串口4（LORA） 是否有数据
        
        Timing_Process();  	 //定时处理任务
                
        Check_Key();				 //按键功能处理
        
        Data_Dispose();      //UM402 LORA 初级处理，串口数据处理			
    }
}			


