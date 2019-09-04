#ifndef __NRFCOMMUNICATION_H
#define __NRFCOMMUNICATION_H
#include "NRF24L01.h"
void NRFAnalyticData(void);
typedef struct
{
	unsigned short int thr;//��������
	unsigned short int thr_zone;
	float pit;//������
	float rol;//�����
	float yaw;//ƫ����
	unsigned char key_l;//�󰴼�
	unsigned char key_r;//�Ұ���	
	
	unsigned char signal_lost_flag : 1;//�źŶ�ʧ��־λ
	unsigned char signal_cnt;//�����źŶ�ʧ����
	
	unsigned char fly_mode;//ģʽ
}_RC_Data;


extern _RC_Data _RC;



#endif



