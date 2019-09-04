#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED1 PBout(6)// PB6  À¶µÆ
#define LED2 PBout(5)// PB5  ºìµÆ
#define LED3 PBout(4)// PB4  À¶µÆ
#define LED4 PBout(3)// PB3  ÂÌµÆ

#define LED_ON  0
#define LED_OFF 1


void LED_Init(void);//³õÊ¼»¯

void LED_show(void);		 				    
#endif
