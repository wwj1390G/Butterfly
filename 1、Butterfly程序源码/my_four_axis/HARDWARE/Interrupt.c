#include "Interrupt.h"
#include "usart.h"
#include "NRFCommunication.h"
#include "mpu6050.h"
#include "imu.h"
#include "pid_controller.h"
#include "APP.h"
#include "query_tast.h"
#include "led.h"
uint32_t running_tim_cnt = 0;

//����ϵͳ����ʱ��
void time_check(_Time_test *running)
{
    running->last_time_us = running->now_time_us;
    running->now_time_us = running_tim_cnt * 5000 + TIM3->CNT;
    
    running->delta_time_us = running->now_time_us - running->last_time_us;
    running->delta_time_ms = running->delta_time_us * 0.001f;
}

_Time_test run_start;
_Time_test run_stop;


void TIM3_IRQHandler(void)
{  
	if(TIM3->SR&0X0001)
	{  
		running_tim_cnt++ ;
		
    time_check(&run_start);
		
		//1��NRF24L01���ݽ���
		NRFAnalyticData();
		
		//2����̬���ݶ�ȡ������
		
		get_gyro_raw();
		get_deg_s(&_gyro_orig_f,&_Mpu.deg_s);
		get_rad_s(&_gyro_orig_f,&_Mpu.rad_s);
	
		get_acc_raw();
	  
		
		acc_iir_lpf(&_acc_orig_f,&_acc_att_lpf,_Mpu.Acc_att_coe);
		get_acc_g(&_acc_att_lpf,&_Mpu.acc_g);
	
		mahony_update(_Mpu.rad_s.x,_Mpu.rad_s.y,_Mpu.rad_s.z,_Mpu.acc_g.x,_Mpu.acc_g.y,_Mpu.acc_g.z);
		
		Matrix_ready();
		
		//3����ѹ���ݶ�ȡ������
 //   acc_iir_lpf(&_acc_orig_f,&_acc_fix_lpf,_Mpu.Acc_alt_coe);
		
		//4���������ݶ�ȡ������
		
		//5��PID�������������
		ControllerMode();//����ģʽ
		ControllerOperation();//PID����
		
		//6�����PWM���
		ControllerOut();//���������
		
		//7��У׼������/���ٶȼ�/����/����
		QueryTast();
		//8��ָʾ����ʾ
		led_duty();
		//9����������վͨ��
		ANO_DMA_DATA();
		
		time_check(&run_stop);
		
		TIM3->SR&=~(1<<0);//���־λ
	}
	
}

