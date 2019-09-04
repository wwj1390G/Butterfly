#ifndef __PID_H
#define __PID_H
#include "stm32f10x.h"

typedef struct
{
	float kp;
	float ki;
	float kd;
}_PID;

typedef struct
{
	_PID Roll;
	_PID Pitch;
	_PID Yaw;
}_PRY;

void PID_Init(void);

extern _PRY _PID_Angle;
extern _PRY _PID_Rate;


#endif

