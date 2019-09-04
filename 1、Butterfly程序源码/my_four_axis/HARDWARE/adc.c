#include "adc.h"
#include "usart.h"
#include "delay.h"
uint16_t adc_value[1];//存储adc采集值
float CellVoltage;//电池电压

static void adc_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE );//使能GPIOC时钟
	
	//PC0 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

static void acd_config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );//使能ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道10,采样时间为239.5周期	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}

/*ADC1 对应DMA1的通道1*/
static void adc_dam_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //将DMA1的通道1寄存器重设为缺省值
	
	DMA_InitStructure.DMA_BufferSize = 1;//通道传输数据量					 			DMA通道的DMA缓存的大小
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
	
	DMA_Cmd(DMA1_Channel1,ENABLE);	
}

void adc_init(void)
{
	adc_gpio_init();
	acd_config();
	adc_dam_config();
}

//电池电压检测处理
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



