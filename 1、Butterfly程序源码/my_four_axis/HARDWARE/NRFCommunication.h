#ifndef __NRFCOMMUNICATION_H
#define __NRFCOMMUNICATION_H
#include "NRF24L01.h"
void NRFAnalyticData(void);
typedef struct
{
	unsigned short int thr;//油门数据
	unsigned short int thr_zone;
	float pit;//俯仰角
	float rol;//横滚角
	float yaw;//偏航角
	unsigned char key_l;//左按键
	unsigned char key_r;//右按键	
	
	unsigned char signal_lost_flag : 1;//信号丢失标志位
	unsigned char signal_cnt;//用于信号丢失计数
	
	unsigned char fly_mode;//模式
}_RC_Data;


extern _RC_Data _RC;



#endif



