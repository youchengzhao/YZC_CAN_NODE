#include "i2c.h"
#include "app.h"
int sw_i2c_init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure; 
   
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
   // Configure I2C1 pins: SCL and SDA 
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	return 0;
}
void TWI_delay(void)
 { 
    uint8_t i=10; 
   while(i--); 
    
 }
int sw_i2c_start()
{
	 SDAH;
  SCLH;
  TWI_delay();
  if(!SDAread)return SWI2C_BUS_BUSY; 
 SDAL;
  TWI_delay();
  if(SDAread) return SWI2C_BUS_ERROR; 
 SCLL;
  TWI_delay();
  return SWI2C_READY;
}
int sw_i2c_stop()
{
  SDAL;
  SCLL;
  TWI_delay();
  SCLH;
  TWI_delay();
  SDAH;
  TWI_delay();
	return 0;
}
int sw_i2c_ack()
{
	SCLL;
  TWI_delay();
  SDAL;
  TWI_delay();
  SCLH;
  TWI_delay();
  SCLL;
  TWI_delay();
	return 0;
}
int sw_i2c_noack()
{
	SCLL;
  TWI_delay();
  SDAH;
  TWI_delay();
  SCLH;
  TWI_delay();
  SCLL;
  TWI_delay();
	return 0;
}

int sw_i2c_wait_ack()// 返回为1有ACK，返回为0无ACK
{
	SCLL;
  TWI_delay();
  SDAH;   
  TWI_delay();
  SCLH;
  TWI_delay();
  if(SDAread)
  {
           SCLL;
           return 0;
  }
  SCLL;
	return 1;
}

int sw_i2c_send_byte(uint8_t SendByte)
{
	uint8_t i=8;
     while(i--)
     {
         SCLL;
         TWI_delay();
         if(SendByte&0x80)
           SDAH;  
         else 
         SDAL;   
         SendByte<<=1;
         TWI_delay();
         SCLH;
         TWI_delay();
     }
     SCLL;
		 return 0;
}
uint8_t sw_i2c_receive_byte()
{
	uint8_t i=8;
     uint8_t ReceiveByte=0;

    SDAH;    
     while(i--)
     {
       ReceiveByte <<= 1;      
       SCLL;
       TWI_delay();
       SCLH;
       TWI_delay(); 
       if(SDAread)
       {
         ReceiveByte |= 0x01;
       }
     }
     SCLL;
     return ReceiveByte;
}


int sw_i2c_read(unsigned char addr_h,unsigned char addr_l,unsigned char *p_buffer,int len)
{
	int i;
	sw_i2c_start();
	sw_i2c_send_byte(ADDR_24CXX & 0XFE);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	sw_i2c_send_byte(addr_h);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	sw_i2c_send_byte(addr_l);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	for (i = 0; i< len; i++)
	{
		sw_i2c_send_byte(p_buffer[i]);
		if (!sw_i2c_wait_ack())
			return SWI2C_BUS_ERROR;
	}
	sw_i2c_stop();
	return 0;
}
int sw_i2c_write(unsigned char addr_h,unsigned char addr_l,unsigned char *p_buffer,int len)
{
	int i;
	sw_i2c_start();
	sw_i2c_send_byte(ADDR_24CXX & 0XFE);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	sw_i2c_send_byte(addr_h);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	sw_i2c_send_byte(addr_l);
	if (!sw_i2c_wait_ack())
	{
		return SWI2C_BUS_ERROR;
	}
	sw_i2c_stop();
	
	sw_i2c_start();
	sw_i2c_send_byte(ADDR_24CXX | 0X01);
	for (i = 0; i< len; i++)
	{
		sw_i2c_send_byte(p_buffer[i]);
		if (!sw_i2c_wait_ack())
			return SWI2C_BUS_ERROR;
	}
	sw_i2c_stop();
	return 0;
}