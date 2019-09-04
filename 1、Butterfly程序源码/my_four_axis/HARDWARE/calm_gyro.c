#include "calm_gyro.h"
#include "calm_acc.h"
#include "iic_eeprom.h"
#include "NRFCommunication.h"
#include "my_math.h"
_GYRO_CAL _gyro_cal = {0};

/*������У׼
*/
#define gyro_cal_sum_value 50
void gyro_cal(_F32xyz *gyro_in)
{
	if(_gyro_cal.flag==1)//��ʼУ׼
	{
//		printf("%2f    %2f    %2f\r\n",_gyro_orig_f.x,_gyro_orig_f.y,_gyro_orig_f.z);
		if(_gyro_cal.i < gyro_cal_sum_value)//��ƽ��ֵ
		{                       
			_gyro_cal.offset.x += gyro_in->x; 
			_gyro_cal.offset.y += gyro_in->y;
			_gyro_cal.offset.z += gyro_in->z;

			_gyro_cal.i++;
		}
		else
		{
			_gyro_cal.i = 0;

			_gyro_cal.offset_flash_write.x = _gyro_cal.offset.x / gyro_cal_sum_value;    //�õ��������ƫ
			_gyro_cal.offset_flash_write.y = _gyro_cal.offset.y / gyro_cal_sum_value;    //�õ��������ƫ
			_gyro_cal.offset_flash_write.z = _gyro_cal.offset.z / gyro_cal_sum_value;    //�õ��������ƫ
			
			set_value(&_gyro_cal.offset,0.0f);//������ݻ���,������У׼ʱ�����ۼ�
			
			printf("gyro    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f\r\n",
																																						_acc_cal.offset_flash_read.x,
																																						_acc_cal.offset_flash_read.y,
																																						_acc_cal.offset_flash_read.z,

																																						_acc_cal.scale_flash_read.x,
																																						_acc_cal.scale_flash_read.y,
																																						_acc_cal.scale_flash_read.z,

																																						_gyro_cal.offset_flash_write.x,
																																						_gyro_cal.offset_flash_write.y,
																																						_gyro_cal.offset_flash_write.z);
			//������������д��flash
			FlashWriteData_gyro(24,_gyro_cal.offset_flash_write.x,
								 _gyro_cal.offset_flash_write.y,
								 _gyro_cal.offset_flash_write.z);   

			_gyro_cal.flash_finish_flag = 1;
			_gyro_cal.flag = 0; //�����׼λ
		}
	}
}



