#ifndef __NRFCOMMUNICATION_H
#define __NRFCOMMUNICATION_H
#include "NRF24L01.h"
void NRFAnalyticData(void);
typedef struct
{
	unsigned short int thr;//ÓÍÃÅ
	unsigned char pit;//¸©Ñö½Ç
	unsigned char rol;//ºá¹ö½Ç
	unsigned char yaw;//º½Ïò½Ç
	
	unsigned int lose_num;
	unsigned int lose_flag;
	
}_RC_Data;


extern _RC_Data _RC;



#endif



