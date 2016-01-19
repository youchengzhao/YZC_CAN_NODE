#ifndef __LCD_H___
#define __LCD_H___

#include "stm32f10x.h"
#define 	LCD_RES			GPIO_Pin_0
#define 	LCD_CS			GPIO_Pin_1
#define 	LCD_RS			GPIO_Pin_2
#define 	LCD_SCK			GPIO_Pin_3
#define 	LCD_SID			GPIO_Pin_4

#define X_1     0X04
#define X_2     0X06
#define X_3     0X00
#define X_4     0X02

#define Y_1     0X00
#define Y_2     0X01
#define Y_3     0X02
#define Y_4     0X03
#define Y_5     0X04
#define Y_6     0X05
#define Y_7     0X06
#define Y_8     0X07

void lcd_gpio_config(void);
void write_lcd_commamd( char CommandByte );
void write_lcd_data( char CommandByte ) ;
void write_lcd_data( char CommandByte ) ;
void lcd_clear( char FillData ) ;
void lcd_init( void ) ;
void  write_32byte(char x,char y,char index);
void  write_16byte(char x,char y,char z,char index);




#endif