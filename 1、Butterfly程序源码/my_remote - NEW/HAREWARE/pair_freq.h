#ifndef __PAIR_FREQ_H
#define __PAIR_FREQ_H
#include "stm32f10x.h"

typedef struct
{
		uint8_t fly_mode;//����ģʽ ��־λ
		uint8_t com_mode;//ͨ��ģʽ ��־λ
    uint8_t addr[5];//NRF��5λͨ�ŵ�ַ
    uint8_t freq_channel[4];
}_PAIR;

void wait_pairing(void);

extern _PAIR pair;


#endif


