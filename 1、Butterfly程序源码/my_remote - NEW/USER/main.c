#include "delay.h"
#include "usart.h"
#include "NRF24L01.h"
#include "led.h"
#include "beep.h"
#include "adc.h"
#include "key.h"
#include "pair_freq.h"
#include "uart1.h"
#include "app.h"
#include "pid.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	

	uart1_init(500000);//DMA-->上位机		
//	uart_init(115200);

	delay_init();
	LED_Init();
	KEY_Init();

	while(NRF24L01_Init())
	{
	}
	NRF24L01_TX_Mode();
	PID_Init();
	adc_init();
	//对频
	//	wait_pairing();
	while(1)
	{
		Change_Mode();
		check_key();

		LED_show();

		
		ANO_DMA_DATA();
	}
}



  


