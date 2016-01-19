#ifndef __I2C_H__
#define  __I2C_H__
#include "stm32f10x.h"
#define ADDR_24CXX        0xA0

#define SCLH         GPIOB->BSRR = GPIO_Pin_6
#define SCLL         GPIOB->BRR  = GPIO_Pin_6 
    
 #define SDAH         GPIOB->BSRR = GPIO_Pin_7
 #define SDAL         GPIOB->BRR  = GPIO_Pin_7

#define SCLread      GPIOB->IDR  & GPIO_Pin_6
#define SDAread      GPIOB->IDR  & GPIO_Pin_7

enum ENUM_TWI_REPLY
 {
  TWI_NACK=0
 ,TWI_ACK=1
 };

enum ENUM_TWI_BUS_STATE
 {
	SWI2C_READY=0
 ,SWI2C_BUS_BUSY=1
 ,SWI2C_BUS_ERROR=2
 };

 
int sw_i2c_init();
int sw_i2c_read(unsigned char addr_h,unsigned char addr_l,unsigned char *p_buffer,int len);
int sw_i2c_write(unsigned char addr_h,unsigned char addr_l,unsigned char *p_buffer,int len);


#endif