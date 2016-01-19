/******************** 鑫盛电子工作室 ********************
 * 文件名  ：main.c
 * 描述    ：给相同两个MINI板下载此程序后，按下一个板子上的按键，可以点亮另一个板子
 *           上对应的LED。两个板子不分主从。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0   																										  
 * 淘宝店：http://shop66177872.taobao.com
*********************************************************/
#include "stm32f10x.h"
#include "can.h"
#include "led.h"
#include "app.h"
#include "lcd.h"
#include "i2c.h"



/*按键管脚初始化*/
void KeyInit(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能外设时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能外设时钟

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //最高输出速率10MHz
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //最高输出速率10MHz
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*检测是否有按键按下*/
void  GetKey(void)
{
        

        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
        {
                delay(1000000);//去抖动//去抖动
                if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
                {
                        while(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)){ ; }//等待按键释放                        
                        can_tx(0X55,0X77); 
						LED1(1);LED2(1);
                }
        }

        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
        {
                 delay(1000000);//去抖动//去抖动
                if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
                {
                        while(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){ ; }//等待按键释放                        
                        can_tx(0X99,0Xbb); 	
						LED1(1);LED2(1);
                }
        }       

      
}



int main(void)
{   
	int * p;
	unsigned char buffer[16]= {1,2,3,4,5,6,7,8,9,0};
	SystemInit(); //设置系统时钟为72M
	KeyInit();	  //按键管脚初始化
	LED_GPIO_Config();//LED管脚初始化
  lcd_gpio_config();
	CAN_GPIO_Config();//CAN管脚初始化
	CAN_NVIC_Configuration(); //CAN中断初始化   
	CAN_INIT();//CA初始化N模块	
//	p = (int*)0x8001600;
//	*p = 0xaaaa;

	lcd_init();
	sw_i2c_init();
	lcd_clear(0x00);
	write_32byte(X_1,Y_1,2);
	write_32byte(X_2,Y_1,2);
	write_32byte(X_3,Y_1,2);
	write_32byte(X_4,Y_1,2);
	sw_i2c_read(0,1,buffer,16);
 	while(1)
    {
	 GetKey();	//检测是否有按键按下
	   
  	}
}




