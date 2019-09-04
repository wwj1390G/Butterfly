#include "led.h"
#include "pair_freq.h"
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//JTAG-DP禁用+SW-DP使能
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable  ,ENABLE);//完全禁用（JTAG+SW-DP)
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO时钟
 	
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//JTAG-DP 失能 + SW-DP使能 
																//使PB3与PB4可做普通I/O口使用
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						    	 
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);
}

void LED_show(void)
{
	
	
	switch(pair.fly_mode)
	{
		case 1:  	LED3=1;LED4=1;;//正常飞行 模式
			break;
		case 2:  	LED3=0;LED4=1;;//定高飞行 模式
			break;
		case 3:		LED3=1;LED4=0;;//定点飞行 模式
			break;
		default : break;
	}
	
	switch(pair.com_mode)
	{
		case 1:  	LED1=0;LED2=0;;//对频模式
			break;
		case 2:  	LED1=1;LED2=1;;//正常通信模式
			break;
		case 3:		LED1=0;LED2=1;;//外环 角度环 PID 调节
			break;
		case 4:		LED1=1;LED2=0;;//内环 角速度环 PID 调节
			break;
		default : break;
	}
}



 
