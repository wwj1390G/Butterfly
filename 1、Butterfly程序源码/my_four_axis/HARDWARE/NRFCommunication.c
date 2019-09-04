#include "NRFCommunication.h"
#include "NRF24L01.h"
#include "my_math.h"
#include "pid.h"

u8 RxData[RX_PLOAD_WIDTH] = {0};
_RC_Data _RC = {0};

void NRFAnalyticData(void)
{
	unsigned char val;
	if(NRF24L01_RxPacket(RxData)==0)
	{
		val = RxData[RX_PLOAD_WIDTH-1];
		switch(val)
		{
			case 0x1A:/*对频数据*/
								if(RxData[0] != 0xA1)
									break;
								_RC.signal_cnt = 0;
				break;
								
								
								
			case 0x2B:/*正常通信数据*/
								if(RxData[0] != 0xB2)
									break;
								
								_RC.signal_cnt = 0;

								_RC.thr = RxData[2]<<8|RxData[1];
								_RC.pit =  (float)(((int8_t)RxData[3]) - 50);
								_RC.rol = -(float)(((int8_t)RxData[4]) - 50);
								_RC.yaw =  (float)(((int8_t)RxData[5]) - 50);
								
								_RC.key_l = RxData[6];
								_RC.key_r = RxData[7];	
								_RC.fly_mode = RxData[8];			

								_RC.thr = LimitProcess(_RC.thr,0,1000);
								_RC.pit = ZeroDispose(_RC.pit,-5,5);
								_RC.rol = ZeroDispose(_RC.rol,-5,5);
								_RC.yaw = ZeroDispose(_RC.yaw,-5,5);   				
								_RC.thr_zone = _RC.thr;
				break;
								
								
								
			case 0x3C:/*外环 角度数据*/
								if(RxData[0] != 0xC3)
									break;
								
								_RC.signal_cnt = 0;
								_RC.thr = RxData[2]<<8|RxData[1];//油门  先发过来的是低8位，再来的是高8位
								
								//PID参数：先发过来的是高8位，再来的是低8位
								all.pit_angle.kp = 0.001f*(RxData[3]<<8 | RxData[4]);
								all.pit_angle.ki = 0.001f*(RxData[5]<<8 | RxData[6]);
								all.pit_angle.kd = 0.001f*(RxData[7]<<8 | RxData[8]);
								
								all.rol_angle.kp = 0.001f*(RxData[9]<<8 | RxData[10]);
								all.rol_angle.ki = 0.001f*(RxData[11]<<8 | RxData[12]);
								all.rol_angle.kd = 0.001f*(RxData[13]<<8 | RxData[14]);
								
								all.yaw_angle.kp = 0.001f*(RxData[15]<<8 | RxData[16]);
								all.yaw_angle.ki = 0.001f*(RxData[17]<<8 | RxData[18]);
								all.yaw_angle.kd = 0.001f*(RxData[19]<<8 | RxData[20]);
				break;
							


			case 0x4D:/*内环 角速度数据*/
								if(RxData[0] != 0xD4)
									break;
								
								_RC.signal_cnt = 0;
								_RC.thr = RxData[2]<<8|RxData[1];//油门  先发过来的是低8位，再来的是高8位
								
								//PID参数：先发过来的是高8位，再来的是低8位
								all.pit_gyro.kp = 0.001f*(RxData[3]<<8 | RxData[4]);
								all.pit_gyro.ki = 0.001f*(RxData[5]<<8 | RxData[6]);
								all.pit_gyro.kd = 0.001f*(RxData[7]<<8 | RxData[8]);
								
								all.rol_gyro.kp = 0.001f*(RxData[9]<<8 | RxData[10]);
								all.rol_gyro.ki = 0.001f*(RxData[11]<<8 | RxData[12]);
								all.rol_gyro.kd = 0.001f*(RxData[13]<<8 | RxData[14]);
								
								all.yaw_gyro.kp = 0.001f*(RxData[15]<<8 | RxData[16]);
								all.yaw_gyro.ki = 0.001f*(RxData[17]<<8 | RxData[18]);
								all.yaw_gyro.kd = 0.001f*(RxData[19]<<8 | RxData[20]);
				break;
			
			
			default :break;
		}		
	}
	else//失联处理
	{
		if(_RC.signal_cnt<200)
		{
			_RC.signal_cnt++;  
			_RC.signal_lost_flag = 0;            
		}

		if(_RC.signal_cnt==200)//信号顶丢失减速    
		{
			_RC.signal_lost_flag = 1;
			if(_RC.thr>10)
			{
				_RC.thr -= 1;		                
			}
		}
		_RC.thr = LimitProcess(_RC.thr,0,1000);
	}

}
	
