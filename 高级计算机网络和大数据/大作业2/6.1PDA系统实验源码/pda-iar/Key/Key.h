#ifndef _Key_H_
#define _Key_H_

#include "stm32f10x.h"



	
#define  AUTO_PRES              1
#define  MANUAL_PRES            2

#define  LOCK_PRES              3
#define  UNLOCK_PRES            4
#define  LOCK_CHECK_PRES        5
#define  REMOVE_ALARM_PRES      6
#define  LIFT_ROD_PRES          7

u8 KEY_Scan(u8 mode);





#endif
