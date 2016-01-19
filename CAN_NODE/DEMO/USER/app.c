#include "app.h"

void set_mem_value(char * buf,int len,char value)
{
	int i;
	for (i = 0; i < len; i++)
		buf[i] = value;
}
void delay( vu32 nCount)
{
	//for (i = 0; i < nCount; i++)
	
  for(; nCount != 0; nCount--);
}

void delay_ms(uint16_t ms)
 {
  uint16_t iq0;
  uint16_t iq1;
  for(iq0 = ms; iq0 > 0; iq0--)
  {
   for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000 
  }
 }