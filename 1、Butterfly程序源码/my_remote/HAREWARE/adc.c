#include <adc.h>
#include <delay.h>
#include "usart.h"
#include "time.h"

unsigned short int adc_value[4];

static void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
static void adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	 
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次转换模式         
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ADC数据右对齐
//	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T4_CC4;//由定时器4，通道4的方波触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//模式为独立模式
	ADC_InitStructure.ADC_NbrOfChannel=4;//通道的数量为4
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//使用扫面模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);//使能ADC1
	ADC_DMACmd(ADC1,ENABLE);//ADC_DMA使能
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC1时钟为6分频
							//72M/6=12，ADC最大时间不能超过14M
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_71Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_71Cycles5);
	
	ADC_ResetCalibration(ADC1);//开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1));//等待复位校准完成

	ADC_StartCalibration(ADC1);//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准完成
	
	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能ADC1的软件转换启动功能
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//外部触发使能

}

static void adc_dma_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //将DMA1的通道1寄存器重设为缺省值
	
	DMA_InitStructure.DMA_BufferSize = 4;//通道传输数据量					 			DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//数据传输方向  		 	外设-->内存
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//是否存储器到存储器方式    		存储器到存储器模式
	DMA_InitStructure.DMA_Mode =DMA_Mode_Circular;//模式：是否循环				循环模式
	DMA_InitStructure.DMA_Priority =DMA_Priority_Medium;//优先级  					优先等级为中
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)adc_value;//存储器基地址						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_HalfWord;//存储器数据宽度		存储器数据长度 半字
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//存储器增量模式      	内存地址增加
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	= ((u32)&ADC1->DR);//外设基地址
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_HalfWord;//外设数据宽度		外设数据长度 半字
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//外设增量模式	   	外设地址固定	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);	
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);			    

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);	
	
	DMA_Cmd(DMA1_Channel1,ENABLE);	
	
	
	//DMA中断优先级配置	
	NVIC_InitTypeDef NVIC_InitStructure;    

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_Init(&NVIC_InitStructure);    
}

void adc_init(void)
{
	adc_gpio_init();
	adc_config();
	adc_dma_config();
	
	TIM4_Init();
}






