#ifndef __PID_CONTROLLER_H
#define __PID_CONTROLLER_H
#include "stm32f10x.h"

typedef struct
{
	uint16_t control_thr;//��������
	uint16_t altitude_thr;//�߶���������
}_Throttle;


void ControllerMode(void);
void ControllerOperation(void);
void ControllerOut(void);

extern _Throttle _Thr;
#endif

