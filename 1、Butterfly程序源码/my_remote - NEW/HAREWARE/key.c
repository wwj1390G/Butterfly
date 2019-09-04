#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "pair_freq.h"
#include "usart.h"  
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB,GPIOC时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO时钟
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//JTAG-DP 失能 + SW-DP使能 
																//使PB3与PB4可做普通I/O口使用

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10.11

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
static u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}

static u8 KEY_Scan2(u8 mode)
{	 
	static u8 key_up=1;
	if(mode)key_up=1; 		  
	if(key_up&&(KEY2==0 && KEY3==0))
	{
		delay_ms(10);
		key_up=0;
		if(KEY2==0 && KEY3 ==0)return 1;
	}else if(KEY2==1&&KEY3==1)key_up=1; 	    
 	return 0;
}

//切换 模式
void Change_Mode(void)
{
	unsigned char val = 0;
	val = KEY_Scan(0);

	if(val == 1)
	{//切换 通信模式
		pair.com_mode++;
	}
	else if(val == 4)
	{//切换 飞行模式
		pair.fly_mode++;
	}
	
	if(pair.com_mode > 4)
	{
		pair.com_mode = 1;
	}
	if(pair.fly_mode == 4)
	{
		pair.fly_mode = 1;
	}
}



_key_ _key = {0};
/*
_key.left1;  			_key.right1;


_key.left2;  			_key.right2;
*/
void check_key(void)
{
	if(pair.com_mode  == 2)//处于正常通信模式
	{
		if(KEY_Scan2(1) == 1)
		{
			_key.left1  = 0xA1;
			_key.right1 = 0x1A; 
			return;
		}
		if(KEY_Scan(1) == 3)
		{
			_key.left1  = 0xA1;
		}
		else
		{
			_key.left1  = 0;
		}
		
		if(KEY_Scan(1) == 2) 
		{
			_key.right1 = 0x1A; 
		}
		else
		{
			_key.right1 = 0;
		}
	}
}


