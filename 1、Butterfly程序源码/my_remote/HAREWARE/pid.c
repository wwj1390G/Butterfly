#include "pid.h"
_PRY _PID_Angle;
_PRY _PID_Rate;

void PID_Init(void)
{
	//外环（角度）
	_PID_Angle.Pitch.kp = 2.8;
	_PID_Angle.Pitch.ki = 0;
	_PID_Angle.Pitch.kd = 0.005;
	
	_PID_Angle.Roll.kp = 2.8;
	_PID_Angle.Roll.ki = 0;
	_PID_Angle.Roll.kd = 0.005;
	
	_PID_Angle.Yaw.kp = 0.001;
	_PID_Angle.Yaw.ki = 0;
	_PID_Angle.Yaw.kd = 0;
	
	//内环（角速度）
	_PID_Rate.Pitch.kp = 0.775;
	_PID_Rate.Pitch.ki = 0.015;
	_PID_Rate.Pitch.kd = 0.009;
	
	_PID_Rate.Roll.kp = 0.775;
	_PID_Rate.Roll.ki = 0.015;
	_PID_Rate.Roll.kd = 0.009;
	
	_PID_Rate.Yaw.kp = 4.2;
	_PID_Rate.Yaw.ki = 0.11;
	_PID_Rate.Yaw.kd = 0.008;
}




