#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED1 PBout(1)// PB1    ���
#define LED2 PBout(6)// PB6  	 �ҵ�


#define LED_ON  0
#define LED_OFF 1


void LED_Init(void);//��ʼ��

void LED_show(void);		 				    
#endif
