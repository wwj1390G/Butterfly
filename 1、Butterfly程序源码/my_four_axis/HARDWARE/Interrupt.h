#ifndef __INTERRUPT_H
#define __INTERRUPT_H
#include "stm32f10x.h"

typedef struct
{
    float       last_time_us;
    float       now_time_us;
    float       delta_time_us;
    float       delta_time_ms;
}_Time_test;   

//测试系统运行时间
void time_check(_Time_test *running);

extern _Time_test att_time;
#endif


