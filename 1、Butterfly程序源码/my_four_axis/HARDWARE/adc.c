#include "adc.h"
#include "usart.h"
#include "delay.h"
uint16_t adc_value[1];//�洢adc�ɼ�ֵ
float CellVoltage;//��ص�ѹ

static void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE );//ʹ��GPIOCʱ��
	
	//PC0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

static void acd_config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );//ʹ��ADC1ͨ��ʱ��

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��10,����ʱ��Ϊ239.5����	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}

/*ADC1 ��ӦDMA1��ͨ��1*/
static void adc_dam_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //��DMA1��ͨ��1�Ĵ�������Ϊȱʡֵ
	
	DMA_InitStructure.DMA_BufferSize = 1;//ͨ������������					 			DMAͨ����DMA����Ĵ�С
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
	
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}

void adc_init(void)
{
	adc_gpio_init();
	acd_config();
	adc_dam_config();
}

//��ص�ѹ��⴦��
void VoltageDetection(void)
{
	unsigned char t;
//	CellVoltage = (float)(adc_value[0]*3.7f*2.0f/4096.0f);
	
	for(t=0;t<50;t++)
	{
		CellVoltage += (float)(adc_value[0]*3.7f*2.0f/4096.0f);
		delay_ms(5);
	}
	CellVoltage = CellVoltage/50;
//	printf("CellVoltage = %4f\r\n",CellVoltage);
//	printf("0x%x\r\n",adc_value[0]);
	
}



