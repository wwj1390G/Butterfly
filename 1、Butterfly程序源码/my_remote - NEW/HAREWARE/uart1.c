#include "uart1.h"
#include "stm32f10x_dma.h"

#include "led.h"
uint8_t TxBuff[32];//�������ݻ�����
uint8_t RxBuff[32];//�������ݻ�����
uint8_t USART_RX_FLAG = 0; //���ڽ��յ����ݱ�־λ
uint8_t USART_TX_FLAG = 0; //���ڷ�����ɱ�־λ
uint16_t USART_RX_Len = 0; //�յ������ݳ���


static void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOAʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����ʱ��ʹ��
	
	/*      I/O��ʼ��   TX   */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//���긴�����
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	/*  RX  */
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//���ջ���������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*      ���ڳ�ʼ��       */
	USART_InitStructure.USART_BaudRate=bound;//������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��ʹ��Ӳ����˵
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//�� ��żУ��
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ 1λ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ַ�����
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);//
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//���������ж�
	
	/*      �����ж����ȼ�       */
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//ѡ��ͨ��1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
}

void uart1_init(u32 bound)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMAʱ��ʹ��
	
	USART1_Init(bound);//����1��ʼ��
	
	/*USART1_RX-����-DMA1 ͨ��5
	 *USART2_RX-����-DMA1 ͨ��6
	*/
	DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_BufferSize			=32;//ͨ������������					 			DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_DIR					=DMA_DIR_PeripheralSRC;//���ݴ��䷽��  		 	�������,��������
	DMA_InitStructure.DMA_M2M					=DMA_M2M_Disable;//�Ƿ�洢�����洢����ʽ    		�洢�����洢��ģʽ
	DMA_InitStructure.DMA_Mode					=DMA_Mode_Normal;//ģʽ���Ƿ�ѭ��				��ͨģʽ,��ѭ��
	DMA_InitStructure.DMA_Priority				=DMA_Priority_Medium;//���ȼ�  					���ȵȼ�Ϊ��
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)RxBuff;//�洢������ַ						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_Byte;//�洢�����ݿ��		�洢�����ݳ���1byte
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//�洢������ģʽ      	�ڴ��ַ����
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	=(u32)&USART1->DR;//�������ַ
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_Byte;//�������ݿ��		�������ݳ���1byte
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//��������ģʽ	   	�����ַ�̶�	
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);//��ʼ�� DMA1 ͨ��6 
	
	
	/*USART1_TX-����-DMA1 ͨ��4
	 *USART2_TX-����-DMA1 ͨ��7
	 */ 
	DMA_DeInit(DMA1_Channel4);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ	
	DMA_InitStructure.DMA_BufferSize			=32;//ͨ������������					 			DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_DIR					=DMA_DIR_PeripheralDST;//���ݴ��䷽��  		 	�����跢������
	DMA_InitStructure.DMA_M2M					=DMA_M2M_Disable;//�Ƿ�洢�����洢����ʽ    		�洢�����洢��ģʽ
	DMA_InitStructure.DMA_Mode					=DMA_Mode_Normal;//ģʽ���Ƿ�ѭ��				��ͨģʽ,��ѭ��
	DMA_InitStructure.DMA_Priority				=DMA_Priority_Medium;//���ȼ�  					���ȵȼ�Ϊ��
	
	DMA_InitStructure.DMA_MemoryBaseAddr		=(u32)TxBuff;//�洢������ַ						
	DMA_InitStructure.DMA_MemoryDataSize		=DMA_MemoryDataSize_Byte;//�洢�����ݿ��		�洢�����ݳ���1byte
	DMA_InitStructure.DMA_MemoryInc				=DMA_MemoryInc_Enable;	//�洢������ģʽ      	�ڴ��ַ����
	
	DMA_InitStructure.DMA_PeripheralBaseAddr	=(u32)&USART1->DR;//�������ַ
	DMA_InitStructure.DMA_PeripheralDataSize	=DMA_PeripheralDataSize_Byte;//�������ݿ��		�������ݳ���1byte
	DMA_InitStructure.DMA_PeripheralInc			=DMA_PeripheralInc_Disable;	//��������ģʽ	   	�����ַ�̶�	
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);//��ʼ�� DMA1 ͨ��7 
	
	/*DMA �ж����ȼ�����*/
	NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel5_IRQn;//USART1_RX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;                //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Channel4_IRQn;//USART1_TX
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;                //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;                //�����ȼ�
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);//�����ж�
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);//�����ж�
	
	
	/*DMAʹ��*/
	DMA_Cmd(DMA1_Channel4,ENABLE);//DMAʹ�� DMA1 ͨ��6 
	DMA_Cmd(DMA1_Channel5,ENABLE);//DMAʹ�� DMA1 ͨ��7
	
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);//����2 DMA��ʼ��
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);//����2 DMA��ʼ��
	
}


//*******************************************
//����һ��DMA����
//*******************************************
void DMA_SendOnce(DMA_Channel_TypeDef* DMAy_Channelx,uint16_t len)
{
	uint32_t j=0;
	
	while((USART_TX_FLAG==1)&&(j<60000))j++;
	
	DMA_Cmd(DMAy_Channelx, DISABLE); //�ر�DMA����
	
	DMA_SetCurrDataCounter(DMAy_Channelx, len);
	USART_TX_FLAG = 1;
	
	DMA_Cmd(DMAy_Channelx, ENABLE); //����DMA����
}


/*
*���ڷ��Ͷ���ֽ�
*data��Ҫ���͵����ݣ����ֽ������Ϊ32
*len �����ݵ��ֽ���
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
//DMA���ڽ�������ж�
//*******************************************
void DMA1_Channel5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC5)!=RESET)
	{
//		LED0=0;
		DMA_Cmd(DMA1_Channel5, DISABLE);//�ر� DMA1 ͨ��1 ����
		
		DMA_SetCurrDataCounter(DMA1_Channel5, 32);
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}



//*******************************************
//���ڿ����ж�
//USART_FLAG=1��ʾ���յ�����
//*******************************************
void USART1_IRQHandler(void)
{
	uint16_t temp;
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		USART_RX_FLAG = 1;
		
		DMA_Cmd(DMA1_Channel5, DISABLE);
		
		temp = USART1->SR;
		temp = USART1->DR;  //�����жϵ�����Ƚ��ر𣬲���д�Ĵ��������Ƕ��Ĵ���

		USART_RX_Len = 32 - DMA_GetCurrDataCounter(DMA1_Channel5);//��ȡ�յ������ݳ���
		
		DMA_SetCurrDataCounter(DMA1_Channel5, 32);//�����������ݴ�����
		
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
}

//*******************************************
//DMA��������ж�  
//*******************************************
void DMA1_Channel4_IRQHandler(void) 
{  
	if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET) 
	{
		USART_TX_FLAG = 0;
		DMA_ClearITPendingBit(DMA1_IT_GL4);//���ͨ��7������ɱ�־
	}
}

/*******************************************************************/






 
