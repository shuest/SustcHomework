#ifndef ALARM_H
#define ALARM_H

extern volatile int flag;

void alarm_init(void);
void onoff_set(unsigned char onoff,char c) ;
#endif