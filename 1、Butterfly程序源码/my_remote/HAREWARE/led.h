#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED1 PBout(6)// PB6  ����
#define LED2 PBout(5)// PB5  ���
#define LED3 PBout(4)// PB4  ����
#define LED4 PBout(3)// PB3  �̵�

#define LED_ON  0
#define LED_OFF 1


void LED_Init(void);//��ʼ��

void LED_show(void);		 				    
#endif
