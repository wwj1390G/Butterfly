#ifndef __PAIR_FREQ_H
#define __PAIR_FREQ_H
#include "stm32f10x.h"

typedef struct
{
		uint8_t fly_mode;//飞行模式 标志位
		uint8_t com_mode;//通信模式 标志位
    uint8_t addr[5];//NRF的5位通信地址
    uint8_t freq_channel[4];
}_PAIR;

void wait_pairing(void);

extern _PAIR pair;


#endif


