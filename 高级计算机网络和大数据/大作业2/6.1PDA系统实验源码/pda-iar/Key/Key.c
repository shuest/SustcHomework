#include "Key.h"

extern uint8_t KeyTimCount;

unsigned char key_up=1;
uint8_t Key_Count = 0;
u8 KEY_Scan(u8 mode)
{	 

	if(mode)

//	key_up=1;   
	if((AUTO==0||MANUAL==0||LOCK==0||UNLOCK==0||LOCK_CHECK==0||REMOVE_ALARM==0||LIFT_ROD==0)&&(key_up!=0))
	{
		Delay_ms(10);
		key_up=0;
		if(AUTO==0)
		{
                  Key_Count++;
                  KeyTimCount = 0;
                  if(Key_Count >= 2)                            //连续按键两下
                  {
                    Key_Count = 0;
                    return AUTO_PRES;
                  }
                }
		else
                {
                  Key_Count = 0;
                  if(MANUAL==0)
                  {return MANUAL_PRES;}
                  
                  else if(LOCK==0)
                  {return LOCK_PRES;}
                  
                  else if(UNLOCK==0)
                  {return UNLOCK_PRES;}
                  
                  else if(LOCK_CHECK==0)
                  {return LOCK_CHECK_PRES;}
                  
                  else if(REMOVE_ALARM==0)
                  {return REMOVE_ALARM_PRES;}
                  
                  else if(LIFT_ROD==0)
                  return LIFT_ROD_PRES;
                }
	}
	else if((AUTO!=0&&MANUAL!=0&&LOCK!=0&&UNLOCK!=0&&LOCK_CHECK!=0&&REMOVE_ALARM!=0&&LIFT_ROD!=0))
	key_up=1; 	    
 	return 0;
}
