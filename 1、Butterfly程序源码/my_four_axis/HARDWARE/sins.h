#ifndef _sins_h_
#define _sins_h_


#include "stm32f10x.h"
#include "mpu6050.h"

#define pos_delay_num  50

typedef struct
{
	_F32xyz state_delta;
	
	_F32xyz last_acc;
	_F32xyz acc;
	_F32xyz acc_cor;
	
	_F32xyz vel;
	_F32xyz vel_org;
	_F32xyz vel_cor;
	_F32xyz vel_delta;
	
	_F32xyz pos;
	_F32xyz pos_org;
	_F32xyz pos_cor;
	_F32xyz pos_history[pos_delay_num];
    
   U16_XYZ pos_delay_cnt;
    
    
    uint16_t high_start_cnt;
}_SINS;


//void sins_high(void);

extern _SINS sins;


#endif

