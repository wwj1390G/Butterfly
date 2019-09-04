#include "calm_mpu.h"
#include "usart.h"
#include "iic_eeprom.h"
#include "my_math.h"
#include "mpu6050.h"
#include "calm_acc.h"

/*��ȡiic_eeprom�б������ֵ
 *���أ� 0 �ɹ�
 *���أ� 1 ʧ��
 */
unsigned char read_iic_eeprom(void)
{
	 unsigned char success = 0;
	//��ʼ�����ϵ������λ���ϵ��
	for(uint8_t i=0;i<3;i++)
	{
		_acc_cal.K[i] = 1.0f;
		_acc_cal.B[i] = 0.0f;
	}
	FlashReadData(0,&_acc_cal.offset_flash_read.x,
									&_acc_cal.offset_flash_read.y,
								  &_acc_cal.offset_flash_read.z, 

								  &_acc_cal.scale_flash_read.x,    
								  &_acc_cal.scale_flash_read.y,
								  &_acc_cal.scale_flash_read.z,

								  &_gyro_cal.offset_flash_read.x,
								  &_gyro_cal.offset_flash_read.y,
								  &_gyro_cal.offset_flash_read.z);
	printf("iiprom  %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f    %0.2f\r\n",
										_acc_cal.offset_flash_read.x,
										_acc_cal.offset_flash_read.y,
										_acc_cal.offset_flash_read.z, 

										_acc_cal.scale_flash_read.x,    
										_acc_cal.scale_flash_read.y,
										_acc_cal.scale_flash_read.z,

										_gyro_cal.offset_flash_read.x,
										_gyro_cal.offset_flash_read.y,
										_gyro_cal.offset_flash_read.z);
	
	// sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
    if(my_abs(_acc_cal.offset_flash_read.x) > 5.0f || my_abs(_acc_cal.offset_flash_read.y) > 5.0f || my_abs(_acc_cal.offset_flash_read.z) > 5.0f)
    {
        success = 1;
    }  
		 // sanity check scale
    if(my_abs(_acc_cal.scale_flash_read.x-1.0f)>0.5f|| my_abs(_acc_cal.scale_flash_read.y-1.0f)>0.5f|| my_abs(_acc_cal.scale_flash_read.z-1.0f)>0.5f)
    {
        success = 2;
    }
		printf("success = %d\r\n",success);
		
		if(success==0)    
    {
        //���ٶȼ� ƫ��
        _acc_cal.B[0] = _acc_cal.offset_flash_read.x;
        _acc_cal.B[1] = _acc_cal.offset_flash_read.y;
        _acc_cal.B[2] = _acc_cal.offset_flash_read.z;
        
        //���ٶȼ� ��������
        _acc_cal.K[0] = _acc_cal.scale_flash_read.x;
        _acc_cal.K[1] = _acc_cal.scale_flash_read.y;
        _acc_cal.K[2] = _acc_cal.scale_flash_read.z;
    }
	
		
		//������ƫ������
    _set_val(&_gyro_offset,&_gyro_cal.offset_flash_read);
		return success;
}





