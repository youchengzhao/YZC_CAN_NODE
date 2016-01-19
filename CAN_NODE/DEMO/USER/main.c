/******************** ��ʢ���ӹ����� ********************
 * �ļ���  ��main.c
 * ����    ������ͬ����MINI�����ش˳���󣬰���һ�������ϵİ��������Ե�����һ������
 *           �϶�Ӧ��LED���������Ӳ������ӡ�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0   																										  
 * �Ա��꣺http://shop66177872.taobao.com
*********************************************************/
#include "stm32f10x.h"
#include "can.h"
#include "led.h"
#include "app.h"
#include "lcd.h"
#include "i2c.h"



/*�����ܽų�ʼ��*/
void KeyInit(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ������ʱ��

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //����������10MHz
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //����������10MHz
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*����Ƿ��а�������*/
void  GetKey(void)
{
        

        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
        {
                delay(1000000);//ȥ����//ȥ����
                if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13))
                {
                        while(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)){ ; }//�ȴ������ͷ�                        
                        can_tx(0X55,0X77); 
						LED1(1);LED2(1);
                }
        }

        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
        {
                 delay(1000000);//ȥ����//ȥ����
                if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12))
                {
                        while(Bit_RESET == GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)){ ; }//�ȴ������ͷ�                        
                        can_tx(0X99,0Xbb); 	
						LED1(1);LED2(1);
                }
        }       

      
}



int main(void)
{   
	int * p;
	unsigned char buffer[16]= {1,2,3,4,5,6,7,8,9,0};
	SystemInit(); //����ϵͳʱ��Ϊ72M
	KeyInit();	  //�����ܽų�ʼ��
	LED_GPIO_Config();//LED�ܽų�ʼ��
  lcd_gpio_config();
	CAN_GPIO_Config();//CAN�ܽų�ʼ��
	CAN_NVIC_Configuration(); //CAN�жϳ�ʼ��   
	CAN_INIT();//CA��ʼ��Nģ��	
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
	 GetKey();	//����Ƿ��а�������
	   
  	}
}




