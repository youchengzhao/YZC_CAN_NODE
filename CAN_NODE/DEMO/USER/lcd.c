#include "lcd.h"
#include "app.h"
#include "lcd_data.h"
static char  ComTable[]={3,2,1,0,7,6,5,4,};

void lcd_gpio_config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟  
  GPIO_InitStructure.GPIO_Pin = (LCD_RES | LCD_CS |LCD_RS | LCD_SCK | LCD_SID);	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PB端口
  //GPIO_SetBits(GPIOA, 0);	 // 关闭所有LED
	GPIO_ResetBits(GPIOA,(LCD_RES | LCD_CS |LCD_RS | LCD_SCK | LCD_SID));	 
	GPIO_SetBits(GPIOA,LCD_RES);
	 
}

#define DELAY_X_NS(x)			delay(5)
// SCK --> minimum period is 50ns
void write_lcd_commamd( char CommandByte ) 
{ 
        char  i; 
      // P1OUT &= ~CS;//CS=0; 
      // P1OUT &= ~RS;//rs=0;         //Command 
			 GPIO_ResetBits(GPIOA,LCD_CS);	 
			 GPIO_ResetBits(GPIOA,LCD_RS);	 
			// GPIO_ResetBits(GPIOA,LCD_SCK);
         for(i=0;i<8;i++) 
       {      	
							
              if((CommandByte>>(7-i)) &0x01)		
               // P1OUT |= SID;
								GPIO_SetBits(GPIOA,LCD_SID);
              else
                //P1OUT &= ~SID;
								GPIO_ResetBits(GPIOA,LCD_SID);
							
             // P1OUT |= SCK;//SCK=1; 
							
							GPIO_SetBits(GPIOA,LCD_SCK);
              //__delay_cycles(20);//_nop_();
							DELAY_X_NS(50);
							GPIO_ResetBits(GPIOA,LCD_SCK);
							DELAY_X_NS(50);
       } 
       //P1OUT |= CS;//CS=0; 
       //P1OUT |= RS;//rs=0;
			// GPIO_SetBits(GPIOA,LCD_CS | LCD_RS); 
			 GPIO_SetBits(GPIOA,LCD_CS);
			 GPIO_SetBits(GPIOA,LCD_RS);	 
			 DELAY_X_NS(50);
			 DELAY_X_NS(50);			 
    
} 


// SCK --> minimum period is 50ns
void write_lcd_data( char CommandByte ) 
{ 
        char  i; 
      // P1OUT &= ~CS;//CS=0; 
      // P1OUT |= RS;//rs=0;         //data
			 GPIO_ResetBits(GPIOA,LCD_CS);	 
			 GPIO_SetBits(GPIOA,LCD_RS);	 
		//	 GPIO_ResetBits(GPIOA,LCD_SCK);
       for(i=0;i<8;i++) 
       {      	
							
              if((CommandByte>>(7-i)) &0x01)		
               // P1OUT |= SID;
								GPIO_SetBits(GPIOA,LCD_SID);
              else
                //P1OUT &= ~SID;
								GPIO_ResetBits(GPIOA,LCD_SID);
							
             // P1OUT |= SCK;//SCK=1; 
							
							GPIO_SetBits(GPIOA,LCD_SCK);
              //__delay_cycles(20);//_nop_();
							DELAY_X_NS(50);
							GPIO_ResetBits(GPIOA,LCD_SCK);
							DELAY_X_NS(50);
       } 
       //P1OUT |= CS;//CS=0; 
       //P1OUT |= RS;//rs=0;
			// GPIO_SetBits(GPIOA,LCD_CS | LCD_RS); 
			 GPIO_SetBits(GPIOA,LCD_CS);
			 GPIO_SetBits(GPIOA,LCD_RS);	 
			 DELAY_X_NS(50);
			 DELAY_X_NS(50);			 
} 

void lcd_clear( char FillData ) 
{ 
       int i,j; 
       for(i=0;i<8;i++) 
       { 
              write_lcd_commamd(0xB0|ComTable[i]);      //Set Page Address 
              write_lcd_commamd(0x10);   //Set Column Address = 0 
              write_lcd_commamd(0x01);   //Colum from S1 -> S128 auto add 
              for(j=0;j<128;j++) 
              {                
								write_lcd_data( FillData ); 								
              } 
       } 
} 
void lcd_init( void ) 
{ 
       write_lcd_commamd(0xE3);   //Display OFF 
       write_lcd_commamd(0xA2);   //1/64 Duty 1/9 Bias 
       write_lcd_commamd(0xA0);   //ADC select S0->S131(????? S1-S128) 
       write_lcd_commamd(0xC8);   //com1 --> com64 
       write_lcd_commamd(0x2F);   //对某些模块没有用 Rb/Ra 
       write_lcd_commamd(0x24);
       write_lcd_commamd(0x81);   //Sets V0 
       write_lcd_commamd(0X21);     //调节对比度
       write_lcd_commamd(0xF8);   //voltage follower ON  regulator ON  booster ON 
       write_lcd_commamd(0x08);   //Normal Display (not reverse dispplay) 
       write_lcd_commamd(0xB0);   //Entire Display Disable      
       write_lcd_commamd(0x10);   //Set Display Start Line = com0 
       write_lcd_commamd(0x00);   //Set Page Address = 0 
       lcd_clear(0); 
       write_lcd_commamd(0xAF);   //Display ON 
} 
void  write_32byte(char x,char y,char index)
{
   char i;
   x &= 0x0f;
   y &= 0x0f;
   write_lcd_commamd(0xB0|x);      //Set Page Address 
   write_lcd_commamd(0x10);   //Set Column Address = 0 
   write_lcd_commamd(0x10|y);   //Colum from S1 -> S128 auto add 
   for(i = 0; i < 16; i++)
   {  
    write_lcd_data(DATA_BUF_32[index][i]);
   }
   x +=1;
   write_lcd_commamd(0xB0|x);      //Set Page Address 
   write_lcd_commamd(0x10);   //Set Column Address = 0 
   write_lcd_commamd(0x10|y);   //Colum from S1 -> S128 auto add 
   for(i = 0; i < 16; i++)
   {
      write_lcd_data(DATA_BUF_32[index+1][i]);
   }
  // WriteData(DATA_BUF[index+1][i]);
}
void  write_16byte(char x,char y,char z,char index)
{
   char i;
   x &= 0x0f;
   y &= 0x0f;
   index = index*2;
   write_lcd_commamd(0xB0|x);      //Set Page Address 
   write_lcd_commamd(0x10 | y);   //Set Column Address = 0 
   write_lcd_commamd(0x00| z);   //Colum from S1 -> S128 auto add 
   for(i = 0; i < 8; i++)
   write_lcd_data(DATA_BUF_16[index][i]);
   x +=1;
   write_lcd_commamd(0xB0|x);      //Set Page Address 
   write_lcd_commamd(0x10 | y);   //Set Column Address = 0 
   write_lcd_commamd(0x00| z);   //Colum from S1 -> S128 auto add
   for(i = 0; i < 8; i++)
   write_lcd_data(DATA_BUF_16[index+1][i]);
}

