#include "uart1.h"
#include "stm32f10x_dma.h"

#include "led.h"
uint8_t TxBuff[32];//发送数据缓存区
uint8_t RxBuff[32];//接收数据缓存区
uint8_t USART_RX_FLAG = 0; //串口接收到数据标志位
uint8_t USART_TX_FLAG = 0; //串口发送完成标志位
uint16_t USART_RX_Len = 0; //收到的数据长度


static void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//串口时钟使能
	
	/*      I/O初始化   TX   */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//推完复用输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	/*  RX  */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空或上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*      串口初始化       */
	USART_InitStructure.USART_BaudRate=bound;//波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//不使用硬件流说
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//无 奇偶校验
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位 1位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字符长度
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);//
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//开启空闲中断
	
	/*      设置中断优先级       */
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//选择通道1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
}

void uart1_init(u32 bound)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA时钟使能
	
	USART1_Init(bound);//串口1初始化
	
	/*USART1_RX-接收-DMA1 通道5
	 *USART2_RX-接收-DMA1 通道6
	*/
	DMA_DeInit(DMA1_Channel5);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_BufferSize			=32;//通道传输数据量					 			DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_DIR					=DMA_DIR_PeripheralSRC;//数据传输方向  		 	从外设读,接收数据
	DMA_InitStructure.DMA_M2M					=DMA_M2M_Disable;//是否存储器到存储器方式    		存储器到存储器模式
	DMA_InitStructure.DMA_Mode					=DMA_Mode_Normal;//模式：是否循环				普通模式,不循环
	DMA_InitStructure.DMA_Priority				=DMA_Priority_Medium;//优先级  					优先等级为中
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)RxBuff;//存储器基地址						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_Byte;//存储器数据宽度		存储器数据长度1byte
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//存储器增量模式      	内存地址增加
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	=(u32)&USART1->DR;//外设基地址
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_Byte;//外设数据宽度		外设数据长度1byte
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//外设增量模式	   	外设地址固定	
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);//初始化 DMA1 通道6 
	
	
	/*USART1_TX-发送-DMA1 通道4
	 *USART2_TX-发送-DMA1 通道7
	 */ 
	DMA_DeInit(DMA1_Channel4);   //将DMA的通道1寄存器重设为缺省值	
	DMA_InitStructure.DMA_BufferSize			=32;//通道传输数据量					 			DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_DIR					=DMA_DIR_PeripheralDST;//数据传输方向  		 	向外设发送数据
	DMA_InitStructure.DMA_M2M					=DMA_M2M_Disable;//是否存储器到存储器方式    		存储器到存储器模式
	DMA_InitStructure.DMA_Mode					=DMA_Mode_Normal;//模式：是否循环				普通模式,不循环
	DMA_InitStructure.DMA_Priority				=DMA_Priority_Medium;//优先级  					优先等级为中
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)TxBuff;//存储器基地址						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_Byte;//存储器数据宽度		存储器数据长度1byte
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//存储器增量模式      	内存地址增加
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	=(u32)&USART1->DR;//外设基地址
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_Byte;//外设数据宽度		外设数据长度1byte
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//外设增量模式	   	外设地址固定	
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);//初始化 DMA1 通道7 
	
	/*DMA 中断优先级配置*/
	NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel5_IRQn;//USART1_RX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;                //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel4_IRQn;//USART1_TX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;                //子优先级
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);//开启中断
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);//开启中断
	
	
	/*DMA使能*/
	DMA_Cmd(DMA1_Channel4,ENABLE);//DMA使能 DMA1 通道6 
	DMA_Cmd(DMA1_Channel5,ENABLE);//DMA使能 DMA1 通道7
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);//串口2 DMA初始化
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);//串口2 DMA初始化
	
}


//*******************************************
//开启一次DMA传输
//*******************************************
void DMA_SendOnce(DMA_Channel_TypeDef* DMAy_Channelx,uint16_t len)
{
	uint32_t j=0;
	
	while((USART_TX_FLAG==1)&&(j<60000))j++;
	
	DMA_Cmd(DMAy_Channelx, DISABLE); //关闭DMA传输
	
	DMA_SetCurrDataCounter(DMAy_Channelx, len);
	USART_TX_FLAG = 1;
	
	DMA_Cmd(DMAy_Channelx, ENABLE); //开启DMA传输
}


/*
*串口发送多个字节
*data：要发送的数据，的字节数最大为32
*len ：数据的字节数
*/

void DMA_SendData(uint8_t *data,uint16_t len)
{
	uint8_t i;
	
	for(i=0;i<len;i++)
	{
		TxBuff[i] = *data;
		data++;
	}
	
	DMA_SendOnce(DMA1_Channel4,len);
}

//*******************************************
//DMA串口接收完成中断
//*******************************************
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5)!=RESET)
	{
//		LED0=0;
		DMA_Cmd(DMA1_Channel5, DISABLE);//关闭 DMA1 通道1 传输
		
		DMA_SetCurrDataCounter(DMA1_Channel5, 32);
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}



//*******************************************
//串口空闲中断
//USART_FLAG=1表示接收到数据
//*******************************************
void USART1_IRQHandler(void)
{
	uint16_t temp;
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		USART_RX_FLAG = 1;
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		temp = USART1->SR;
		temp = USART1->DR;  //空闲中断的清除比较特别，不是写寄存器，而是读寄存器

		USART_RX_Len = 32 - DMA_GetCurrDataCounter(DMA1_Channel5);//获取收到的数据长度
		
		DMA_SetCurrDataCounter(DMA1_Channel5, 32);//重新设置数据传输量
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

//*******************************************
//DMA发送完成中断  
//*******************************************
void DMA1_Channel4_IRQHandler(void) 
{  
	if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET) 
	{
		USART_TX_FLAG = 0;
		DMA_ClearITPendingBit(DMA1_IT_GL4);//清除通道7传输完成标志
	}
}

/*******************************************************************/






 
