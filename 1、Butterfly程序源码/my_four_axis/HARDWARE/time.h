#ifndef _TIME_H
#define _TIME_H
#include "sys.h"


void TIM3_Init(void);
void TIM4_PWM_Init(void);
void MoterPower(uint16_t MOTO1_PWM,uint16_t MOTO2_PWM,uint16_t MOTO3_PWM,uint16_t MOTO4_PWM);


#endif


