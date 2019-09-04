#include "controller.h"
#include "query_tast.h"
#include "NRFCommunication.h"
#include "pid_controller.h"
#include "pid.h"
#include "time.h"
#define ThrottleThreshold  20

uint16_t moter1 = 0;
uint16_t moter2 = 0;
uint16_t moter3 = 0;
uint16_t moter4 = 0;
#define limit_thr = 20;
/*控制量输出*/
void ControllerOut(void)
{
	if(_lock.lock_flag == 1)
	{
		if(_RC.thr > 20)
		{	
			moter1 = _Thr.control_thr + all.pit_gyro.Output - all.rol_gyro.Output - all.yaw_gyro.Output;
			moter2 = _Thr.control_thr + all.pit_gyro.Output + all.rol_gyro.Output + all.yaw_gyro.Output; 
			moter3 = _Thr.control_thr - all.pit_gyro.Output + all.rol_gyro.Output - all.yaw_gyro.Output;
			moter4 = _Thr.control_thr - all.pit_gyro.Output - all.rol_gyro.Output + all.yaw_gyro.Output;  
			
//			moter1 = _Thr.control_thr;
//			moter2 = _Thr.control_thr;
//			moter3 = _Thr.control_thr;
//			moter4 = _Thr.control_thr; 
		}
		else
		{
			moter1 = _RC.thr;
			moter2 = _RC.thr;
			moter3 = _RC.thr;
			moter4 = _RC.thr;
		
			/*清积分*/
			clear_integral(&all.pit_angle);
			clear_integral(&all.pit_gyro);    
			clear_integral(&all.rol_angle);
			clear_integral(&all.rol_gyro);
			clear_integral(&all.yaw_angle);
			clear_integral(&all.yaw_gyro);
		}
		MoterPower(moter1,moter2,moter3,moter4);
	}
	else
	{
		MoterPower(0,0,0,0);
		/*清积分*/
		clear_integral(&all.pit_angle);
		clear_integral(&all.pit_gyro);    
		clear_integral(&all.rol_angle);
		clear_integral(&all.rol_gyro);
		clear_integral(&all.yaw_angle);
		clear_integral(&all.yaw_gyro);
	}
}

