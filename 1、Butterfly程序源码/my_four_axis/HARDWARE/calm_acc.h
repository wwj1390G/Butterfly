#ifndef __CALM_ACC_H
#define __CALM_ACC_H

#include "stm32f10x.h"
#include "mpu6050.h"

typedef struct
{
//	uint16_t cnt;
	uint16_t single_cnt;
	uint8_t  single;//����У׼��־λ
	uint32_t i;
//	uint8_t  flag;
	uint8_t  return_flag;//���ٶȼ�У׼���ɹ�/ʧ�ܱ�־λ
//	uint8_t  start_flag;    
	uint8_t  single_finish_flag[7];//1-6��У׼��ɱ�־λ
	uint8_t all_finish_flag;//���������ݲɼ���ɱ�־λ
	_F32xyz samples_sum;
	_F32xyz samples[7];

	_F32xyz offset_flash_read;
	_F32xyz offset_flash_write;
	_F32xyz scale_flash_read;
	_F32xyz scale_flash_write;

	uint8_t flash_finish_flag;//���ٶȼ�У׼���
	
	float B[3];
	float K[3];
}_ACC_CAL;



extern _ACC_CAL _acc_cal;

//���ٶȼ�У׼ 
void acc_cal(_F32xyz *acc_in);
//���ٶȼ�У׼��ѯ
//void acc_cal_polling(void);
#endif

