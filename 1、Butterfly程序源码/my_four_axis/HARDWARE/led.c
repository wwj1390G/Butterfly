#include "led.h"
#include "query_tast.h"
#include "calm_mpu.h"
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //PB0.1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	//PC1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 
	GPIO_SetBits(GPIOC,GPIO_Pin_1); 			
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁用JTAG；使能SWD
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable  ,ENABLE);//完全禁用（JTAG；SWD）
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}

void led_all_on(void)
{
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
}
void led_all_off(void)
{
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
}

/*led闪烁频率*/
uint16_t led_freq_num =0;
#define led_freq  50    //led_freq *2*5ms

/*led闪烁周期*/
uint16_t led_period_num =0;
#define led_period  10 //led_freq*2*5*20 ms = led_freq/5 s 
static unsigned char led_twinkle(uint8_t led1,uint8_t led2,uint8_t led3,uint8_t led4)
{
	led_freq_num++;
	
	if(led_freq_num >= led_freq)
	{
		led_freq_num = 0;
		if(led1) {LED1 = !LED1;} else {LED1 = 1;}
		if(led2) {LED2 = !LED2;} else {LED2 = 1;}
		if(led3) {LED3 = !LED3;} else {LED3 = 1;}
		if(led4) {LED4 = !LED4;} else {LED4 = 1;}
		led_period_num++;
		if(led_period_num >= led_period)
		{
			led_period_num = 0;
			return 1;
		}	
	}
	return 0;
}

void led_duty(void)
{
	unsigned char flag = 0;
	
	/*解锁指示灯*/
	if(_lock.light == 1)
	{
		flag = led_twinkle(1,1,1,1);	
		if(flag == 1)
		{
			_lock.light = 0;
		}
	}
	/*上锁指示灯*/
	else if(_lock.light == 2)
	{
		flag = led_twinkle(0,1,0,1);	
		if(flag == 1)
		{
			_lock.light = 0;
		}
	}
	
	/*陀螺仪校准*/
	else if(_gyro_cal.flag == 1)
	{
		flag = led_twinkle(1,0,1,0);	
//		if(flag == 1)
//		{
//			_gyro_cal.flag = 0;
//		}
	}
	
	/*加速度计 正面校准*/
	else if(_acc_cal.single == 1)
	{
		flag = led_twinkle(0,0,0,1);	
	}
	
	/*加速度计 背面校准*/
	else if(_acc_cal.single == 6)
	{
		flag = led_twinkle(0,0,1,0);	
	}
	/*加速度计 前面校准*/
	else if(_acc_cal.single == 4)
	{
		flag = led_twinkle(1,0,0,1);	
	}
	/*加速度计 后面校准*/
	else if(_acc_cal.single == 5)
	{
		flag = led_twinkle(0,1,1,0);	
	}
	/*加速度计 左面校准*/
	else if(_acc_cal.single == 2)
	{
		flag = led_twinkle(1,1,0,0);	
	}
	/*加速度计 右面校准*/
	else if(_acc_cal.single == 3)
	{
		flag = led_twinkle(0,0,1,1);	
	}	
	
	if(_gyro_cal.flash_finish_flag ==1 && _acc_cal.flash_finish_flag ==1)
	{
		led_twinkle(1,1,1,1);	
		/*灯一直闪烁，断电重启*/
	}
	
}
 
