#ifndef __CALM_GYRO_H
#define __CALM_GYRO_H

#include "stm32f10x.h"
#include "mpu6050.h"


typedef struct
{
//    uint16_t cnt;
    uint16_t i;
    
    uint8_t  flag;
//    uint8_t  start_flag;    
    uint8_t    flash_finish_flag;    
//    uint32_t sum;
    
    _F32xyz offset;
    _F32xyz offset_flash_read;
    _F32xyz offset_flash_write;    
}_GYRO_CAL;

extern _GYRO_CAL _gyro_cal;

void gyro_cal(_F32xyz *gyro_in);//ÍÓÂÝÒÇÆ«ÁãÐ£×¼

#endif

