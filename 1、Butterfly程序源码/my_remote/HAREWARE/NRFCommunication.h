#ifndef __NRFCOMMUNICATION_H
#define __NRFCOMMUNICATION_H
#include "NRF24L01.h"
void NRFAnalyticData(void);
typedef struct
{
	unsigned short int thr;//����
	unsigned char pit;//������
	unsigned char rol;//�����
	unsigned char yaw;//�����
	
	unsigned int lose_num;
	unsigned int lose_flag;
	
}_RC_Data;


extern _RC_Data _RC;



#endif



