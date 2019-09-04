#include "led.h"
#include "pair_freq.h"
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//JTAG-DP����+SW-DPʹ��
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable  ,ENABLE);//��ȫ���ã�JTAG+SW-DP)
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ��AFIOʱ��
 	
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//JTAG-DP ʧ�� + SW-DPʹ�� 
																//ʹPB3��PB4������ͨI/O��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						    	 
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
}

void LED_show(void)
{
	
	
	switch(pair.fly_mode)
	{
		case 1:  	LED3=1;LED4=1;;//�������� ģʽ
			break;
		case 2:  	LED3=0;LED4=1;;//���߷��� ģʽ
			break;
		case 3:		LED3=1;LED4=0;;//������� ģʽ
			break;
		default : break;
	}
	
	switch(pair.com_mode)
	{
		case 1:  	LED1=0;LED2=0;;//��Ƶģʽ
			break;
		case 2:  	LED1=1;LED2=1;;//����ͨ��ģʽ
			break;
		case 3:		LED1=0;LED2=1;;//�⻷ �ǶȻ� PID ����
			break;
		case 4:		LED1=1;LED2=0;;//�ڻ� ���ٶȻ� PID ����
			break;
		default : break;
	}
}



 
