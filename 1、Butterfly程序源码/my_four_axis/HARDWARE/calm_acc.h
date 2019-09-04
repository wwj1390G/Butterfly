#ifndef __CALM_ACC_H
#define __CALM_ACC_H

#include "stm32f10x.h"
#include "mpu6050.h"

typedef struct
{
//	uint16_t cnt;
	uint16_t single_cnt;
	uint8_t  single;//单面校准标志位
	uint32_t i;
//	uint8_t  flag;
	uint8_t  return_flag;//加速度计校准，成功/失败标志位
//	uint8_t  start_flag;    
	uint8_t  single_finish_flag[7];//1-6面校准完成标志位
	uint8_t all_finish_flag;//所有面数据采集完成标志位
	_F32xyz samples_sum;
	_F32xyz samples[7];

	_F32xyz offset_flash_read;
	_F32xyz offset_flash_write;
	_F32xyz scale_flash_read;
	_F32xyz scale_flash_write;

	uint8_t flash_finish_flag;//加速度计校准完成
	
	float B[3];
	float K[3];
}_ACC_CAL;



extern _ACC_CAL _acc_cal;

//加速度计校准 
void acc_cal(_F32xyz *acc_in);
//加速度计校准查询
//void acc_cal_polling(void);
#endif

