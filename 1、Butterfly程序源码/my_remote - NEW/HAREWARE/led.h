#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED1 PBout(1)// PB1    ×óµÆ
#define LED2 PBout(6)// PB6  	 ÓÒµÆ


#define LED_ON  0
#define LED_OFF 1


void LED_Init(void);//³õÊ¼»¯

void LED_show(void);		 				    
#endif
