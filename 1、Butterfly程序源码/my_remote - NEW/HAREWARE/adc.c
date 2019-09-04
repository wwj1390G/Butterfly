#include <adc.h>
#include <delay.h>
#include "usart.h"
#include "time.h"

unsigned short int adc_value[4];

static void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
static void adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	 
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//����ת��ģʽ         
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ADC�����Ҷ���
//	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T4_CC4;//�ɶ�ʱ��4��ͨ��4�ķ�������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ģʽΪ����ģʽ
	ADC_InitStructure.ADC_NbrOfChannel=4;//ͨ��������Ϊ4
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ʹ��ɨ��ģʽ
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);//ʹ��ADC1
	ADC_DMACmd(ADC1,ENABLE);//ADC_DMAʹ��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC1ʱ��Ϊ6��Ƶ
							//72M/6=12��ADC���ʱ�䲻�ܳ���14M
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_71Cycles5);	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_71Cycles5);
	
	ADC_ResetCalibration(ADC1);//������λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ���λУ׼���

	ADC_StartCalibration(ADC1);//����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼���
	
	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ADC1�����ת����������
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//�ⲿ����ʹ��

}

static void adc_dma_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //��DMA1��ͨ��1�Ĵ�������Ϊȱʡֵ
	
	DMA_InitStructure.DMA_BufferSize = 4;//ͨ������������					 			DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���ݴ��䷽��  		 	����-->�ڴ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�Ƿ�洢�����洢����ʽ    		�洢�����洢��ģʽ
	DMA_InitStructure.DMA_Mode =DMA_Mode_Circular;//ģʽ���Ƿ�ѭ��				ѭ��ģʽ
	DMA_InitStructure.DMA_Priority =DMA_Priority_Medium;//���ȼ�  					���ȵȼ�Ϊ��
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)adc_value;//�洢������ַ						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_HalfWord;//�洢�����ݿ��		�洢�����ݳ��� ����
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//�洢������ģʽ      	�ڴ��ַ����
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	= ((u32)&ADC1->DR);//�������ַ
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_HalfWord;//�������ݿ��		�������ݳ��� ����
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//��������ģʽ	   	�����ַ�̶�	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);	
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);			    

	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);	
	
	DMA_Cmd(DMA1_Channel1,ENABLE);	
	
	
	//DMA�ж����ȼ�����	
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






