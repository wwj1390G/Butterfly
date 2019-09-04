#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "pair_freq.h"
#include "usart.h"  
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOB,GPIOCʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//ʹ��AFIOʱ��
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//JTAG-DP ʧ�� + SW-DPʹ�� 
																//ʹPB3��PB4������ͨI/O��ʹ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10.11

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
static u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(KEY4==0)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// �ް�������
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

//�л� ģʽ
void Change_Mode(void)
{
	unsigned char val = 0;
	val = KEY_Scan(0);

	if(val == 1)
	{//�л� ͨ��ģʽ
		pair.com_mode++;
	}
	else if(val == 4)
	{//�л� ����ģʽ
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
	if(pair.com_mode  == 2)//��������ͨ��ģʽ
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


