#ifndef __DMA_H
#define __DMA_H
#include<STM32F10x.H>

extern uint8_t TxBuff[32];//�������ݻ�����
extern uint8_t RxBuff[32];//�������ݻ�����
extern uint8_t USART_RX_FLAG; //���ڽ��յ����ݱ�־λ


void uart2_init(u32 bound);//DMA ��ʼ��

void DMA_SendData(uint8_t *data,uint16_t len);//���ڷ��Ͷ��ֽ�

#endif

