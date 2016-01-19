/******************** ��ʢ���ӹ����� ********************
 * �ļ���  ��can.c
 * ����    ��CAN����Ӧ�ú����⡣         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�-----------------
 *          | 				  |
            |  PB8-CAN-RX     |
 *          |  PB9-CAN-TX     |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.0.0  																										  
 * �Ա��꣺http://shop66177872.taobao.com
*********************************************************/
#include "can.h"
#include "led.h"
#include "stdio.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* ���жϴ������з��� */
__IO uint32_t ret = 0;

volatile TestStatus TestRx;	

/*CAN RX0 �ж����ȼ�����  */
 void CAN_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/*CAN GPIO ��ʱ������ */
 void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* ���ù��ܺ�GPIOB�˿�ʱ��ʹ��*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	                        											 

  /* CAN1 ģ��ʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

  /* Configure CAN pin: RX */	 // PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   // PB9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 ��ʵ��û���õ���ӳ��I/O
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

  
}

/*	CAN��ʼ�� */
 void CAN_INIT(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;  

  /* CAN register init */
  CAN_DeInit(CAN1);	//������CAN��ȫ���Ĵ�������Ϊȱʡֵ
  CAN_StructInit(&CAN_InitStructure);//��CAN_InitStruct�е�ÿһ��������ȱʡֵ����

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//û��ʹ��ʱ�䴥��ģʽ
  CAN_InitStructure.CAN_ABOM=DISABLE;//û��ʹ���Զ����߹���
  CAN_InitStructure.CAN_AWUM=DISABLE;//û��ʹ���Զ�����ģʽ
  CAN_InitStructure.CAN_NART=DISABLE;//û��ʹ�ܷ��Զ��ش�ģʽ
  CAN_InitStructure.CAN_RFLM=DISABLE;//û��ʹ�ܽ���FIFO����ģʽ
  CAN_InitStructure.CAN_TXFP=DISABLE;//û��ʹ�ܷ���FIFO���ȼ�
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//CAN����Ϊ����ģʽ
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //����ͬ����Ծ���1��ʱ�䵥λ
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq; //ʱ���1Ϊ3��ʱ�䵥λ
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq; //ʱ���2Ϊ2��ʱ�䵥λ
  CAN_InitStructure.CAN_Prescaler=60;  //ʱ�䵥λ����Ϊ60	
  CAN_Init(CAN1,&CAN_InitStructure);
                                      //������Ϊ��72M/2/60(1+3+2)=0.1 ��100K

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=1;//ָ��������Ϊ1
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//ָ��������Ϊ��ʶ������λģʽ
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//������λ��Ϊ32λ
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;// ��������ʶ���ĸ�16λֵ
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//	 ��������ʶ���ĵ�16λֵ
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//���������α�ʶ���ĸ�16λֵ
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//	���������α�ʶ���ĵ�16λֵ
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;// �趨��ָ���������FIFOΪ0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;// ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);//	������Ĳ�����ʼ��������

  /* CAN FIFO0 message pending interrupt enable */ 
 CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); //ʹ��FIFO0��Ϣ�Һ��ж�
 
 }  
/* ���������ֽڵ�����*/
void can_tx(u8 Data1,u8 Data2)
{ 
  CanTxMsg TxMessage;  

  TxMessage.StdId=0x00;	//��׼��ʶ��Ϊ0x00
  TxMessage.ExtId=0x0000; //��չ��ʶ��0x0000
  TxMessage.IDE=CAN_ID_EXT;//ʹ�ñ�׼��ʶ��
  TxMessage.RTR=CAN_RTR_DATA;//Ϊ����֡
  TxMessage.DLC=2;	//	��Ϣ�����ݳ���Ϊ2���ֽ�
  TxMessage.Data[0]=Data1; //��һ���ֽ�����
  TxMessage.Data[1]=Data2; //�ڶ����ֽ����� 
  CAN_Transmit(CAN1,&TxMessage); //��������
  
 
}
/* USB�жϺ�CAN�����жϷ������USB��CAN����I/O������ֻ�õ�CAN���жϡ� */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  
  CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;    

  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); //����FIFO0�е�����  
 
  if((RxMessage.Data[0]==0x99)&&(RxMessage.Data[1]==0xbb))   
  { LED1(0);LED2(1);}
   if((RxMessage.Data[0]==0x55)&&(RxMessage.Data[1]==0x77))   
  { LED1(1);LED2(0);}
   
  
} 


