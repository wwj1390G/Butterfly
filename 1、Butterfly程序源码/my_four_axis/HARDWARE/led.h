#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

/*
LED0    LED3

LED1		LED2
*/
#define LED1 PAout(15)
#define LED2 PCout(1)
#define LED3 PBout(0)
#define LED4 PBout(1)

void LED_Init(void);//≥ı ºªØ

void led_all_on(void);
void led_all_off(void);
void led_duty(void);
	 				    
#endif
