#ifndef __DMA_H
#define __DMA_H
#include<STM32F10x.H>

extern uint8_t TxBuff[32];//发送数据缓存区
extern uint8_t RxBuff[32];//接收数据缓存区
extern uint8_t USART_RX_FLAG; //串口接收到数据标志位


void uart2_init(u32 bound);//DMA 初始化

void DMA_SendData(uint8_t *data,uint16_t len);//串口发送多字节

#endif

