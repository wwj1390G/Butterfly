#include "query_tast.h"
#include "NRFCommunication.h"
#include "calm_gyro.h"
#include "calm_acc.h"
#include "led.h"
#include "my_math.h"
#include "iic_eeprom.h"
// ������־λ
_LOCK _lock;

void QueryTast(void)
{
	static unsigned int num = 0;
	num++;

	if(_RC.key_l==0xA1 &&  _RC.key_r==0x1A)//������У׼
	{
			if(num>=600)
			{
				_gyro_cal.flag = 1;
//				_gyro_cal.start_flag = 1;//��Ϊ������У׼ʱ״̬��ʾʹ��
				num = 0;
			}

	}
	else if(_RC.key_l == 0xA1 && _RC.key_r == 0)
	{
			/*����*/
			if(_RC.rol<-40 && _RC.pit>-10 && _RC.pit<10)
			{
				//5ms * 600     ����3s
				if(num>=600)
				{
					_lock.lock_flag = 1;
					_lock.light = 1;
					num=0;
				}
			}	
			/*����*/
			else if(_RC.rol>40 && _RC.pit>-10 && _RC.pit<10)
			{
					//5ms * 600     ����3s
				if(num>=600)
				{
					_lock.lock_flag = 2;
					_lock.light = 1;
					num=0;
				}
			}
			//���ٶȼ�У׼����
			else if(_RC.pit>40 && _RC.rol>-10 && _RC.rol<10)
			{
				//5ms * 300     ����1.5s
				if(num>=300)
				{
					_acc_cal.single = 1;
					num=0;
				}
			}
			//���ٶȼ�У׼����
			else if(_RC.pit<-40 && _RC.rol>-10 && _RC.rol<10)
			{
				//5ms * 300     ����1.5s
				if(num>=300)
				{
					_acc_cal.single = 6;
					num=0;
				}
			}
			
	}
	else if(_RC.key_l == 0 && _RC.key_r == 0x1A)
	{
		//���ٶȼ�У׼ǰ��
		if(_RC.thr>950 && _RC.yaw>-10 && _RC.yaw<10)
		{	
			//5ms * 300     ����1.5s
			if(num>=300)
			{
				_acc_cal.single = 4;
				num=0;
			}
		}
		
		//���ٶȼ�У׼�����
		else if(_RC.thr<50 && _RC.yaw>-10 && _RC.yaw<10)
		{
			//5ms * 300     ����1.5s
			if(num>=300)
			{
				_acc_cal.single = 5;
				num=0;
			}
		}
		
		//���ٶȼ�У׼������
		else if(_RC.yaw>40 && _RC.thr>400 && _RC.thr<600)
		{
			//5ms * 300     ����1.5s
			if(num>=300)
			{
				_acc_cal.single = 2;
				num=0;
			}
		}
		
		//���ٶȼ�У׼������
		else if(_RC.yaw<-40 && _RC.thr>400 && _RC.thr<600)
		{
			//5ms * 300     ����1.5s
			if(num>=300)
			{
				_acc_cal.single = 3;
				num=0;
			}
		}	
	}
	else
	{
		num = 0;
	}	
}


