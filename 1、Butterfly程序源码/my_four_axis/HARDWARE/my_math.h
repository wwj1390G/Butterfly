#ifndef __MY_MATH_H
#define __MY_MATH_H
#include "stm32f10x.h"
#include "mpu6050.h"
uint16_t  LimitProcess(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max);
float ZeroDispose(float in_dat,float min_dat,float max_dat);
float invSqrt(float x);
float my_abs(float f);
void _set_val(_F32xyz *_out_data,_F32xyz *_in_data);
void  set_value(_F32xyz *_in_data,float value);
#endif

