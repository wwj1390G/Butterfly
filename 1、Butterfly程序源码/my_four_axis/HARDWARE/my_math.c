#include "my_math.h"

/*限幅处理*/
uint16_t  LimitProcess(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max)
{
	if(thr_in<thr_min)	thr_in = thr_min;
	if(thr_in>thr_max)	thr_in = thr_max;
	
	return thr_in;
}

/*归零限幅处理*/
float ZeroDispose(float in_dat,float min_dat,float max_dat)
{
    if(in_dat>min_dat&&in_dat<max_dat)  
        in_dat = 0;
    
    return in_dat;
}

// Fast inverse square-root

float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


float my_abs(float f)
{
	if (f >= 0.0f)
	{
		return f;
	}

	return -f;
}

void _set_val(_F32xyz *_out_data,_F32xyz *_in_data)
{
    _out_data->x = _in_data->x;
    _out_data->y = _in_data->y;
    _out_data->z = _in_data->z;
}


void  set_value(_F32xyz *_in_data,float value)
{
    _in_data->x = value;
    _in_data->y = value;
    _in_data->z = value;
}

