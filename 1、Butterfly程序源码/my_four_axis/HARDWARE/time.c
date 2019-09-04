#include "time.h"
#include "my_math.h"
/*ϵͳ��ʱ��  5ms*/
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
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ�ܶ�ʱ��3�� �����ж�
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//TIM3 �ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;//�����ȼ�3
	NVIC_Init(&NVIC_InitStructure);//�ж����ȼ�����
	
	TIM_Cmd(TIM3,ENABLE);	
}



//arr=1000-1=999  psc=8-1=7
void TIM4_PWM_Init(void)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4ʱ��ʹ��    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //���츴�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�100MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	  
	//��ʱ������ t = 1000*8/72 (us) = 1/9000(s)     Ƶ�� = 9khz
	TIM_TimeBaseStructure.TIM_Prescaler=7;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=999;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
	
	
	
	//��ʼ��TIM4 Channel-4 PWMģʽ	 	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//����ʱΪ�͵�ƽ
	TIM_OCInitStructure.TIM_Pulse = 0;									//??TIM1?CCR?

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 ͨ��1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 ͨ��2	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 ͨ��1	 	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 ͨ��2
	

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM14						  
} 

void MoterPower(uint16_t MOTO1_PWM,uint16_t MOTO2_PWM,uint16_t MOTO3_PWM,uint16_t MOTO4_PWM)
{
	TIM4->CCR1 = LimitProcess(MOTO1_PWM,0,1000);
	TIM4->CCR2 = LimitProcess(MOTO2_PWM,0,1000);
	TIM4->CCR3 = LimitProcess(MOTO3_PWM,0,1000);
	TIM4->CCR4 = LimitProcess(MOTO4_PWM,0,1000);
}






