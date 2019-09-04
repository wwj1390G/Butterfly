#include "time.h"
#include "my_math.h"
/*系统定时器  5ms*/
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_timeBaseStucture;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_timeBaseStucture.TIM_ClockDivision = TIM_CKD_DIV1;   
	TIM_timeBaseStucture.TIM_Prescaler = 72-1;                	
	TIM_timeBaseStucture.TIM_Period = 5000-1;	
	TIM_timeBaseStucture.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM3,&TIM_timeBaseStucture);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能定时器3的 更新中断
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//子优先级3
	NVIC_Init(&NVIC_InitStructure);//中断优先级分组
	
	TIM_Cmd(TIM3,ENABLE);	
}



//arr=1000-1=999  psc=8-1=7
void TIM4_PWM_Init(void)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4时钟使能    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//使能PORTF时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	  
	//定时器周期 t = 1000*8/72 (us) = 1/9000(s)     频率 = 9khz
	TIM_TimeBaseStructure.TIM_Prescaler=7;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=999;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
	
	
	
	//初始化TIM4 Channel-4 PWM模式	 	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//空闲时为低电平
	TIM_OCInitStructure.TIM_Pulse = 0;									//??TIM1?CCR?

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 通道1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 通道2	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 通道1	 	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 通道2
	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM14						  
} 

void MoterPower(uint16_t MOTO1_PWM,uint16_t MOTO2_PWM,uint16_t MOTO3_PWM,uint16_t MOTO4_PWM)
{
	TIM4->CCR1 = LimitProcess(MOTO1_PWM,0,1000);
	TIM4->CCR2 = LimitProcess(MOTO2_PWM,0,1000);
	TIM4->CCR3 = LimitProcess(MOTO3_PWM,0,1000);
	TIM4->CCR4 = LimitProcess(MOTO4_PWM,0,1000);
}






