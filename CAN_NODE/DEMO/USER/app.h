#ifndef __APP_H__
#define  __APP_H__
#include "stm32f10x.h"
void set_mem_value(char * buf,int len,char value);
void delay(vu32 nCount);
void delay_ms(uint16_t ms);
#endif
 
