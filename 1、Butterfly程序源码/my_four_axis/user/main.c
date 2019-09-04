#include "delay.h"
#include "usart.h"
#include "uart2.h"
#include "led.h"
#include "mpu6050.h"
#include "bmp280.h"
#include "NRF24L01.H"
#include "iic_eeprom.h"
#include "time.h"
#include "adc.h"
#include "pid.h"
#include "calm_mpu.h"


/*****************************
		       机头
M1(逆时针)       M3(顺时针)          
			 *     X     *
        *    |    *
         *   |   *
          *  |  *
           * | *
 					  * *
  Y--------- *
					  * *
					 *	 *
          *     *
         *       *
        *         *
       *           *						 
M2(顺时针)  		 M4(逆时针)  
		       机尾
					 
X、Y ：是MPU6050的方向
*****************************/

void system(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	uart_init(115200);
	uart2_init(115200);
	delay_init();
	LED_Init();
	adc_init();
	
	all_pid_init();
	get_iir_factor(&_Mpu.Acc_att_coe,0.005f,15);//姿态解算时 加速度 低通系数
	
	MPU_IIC_Init();
	while(MPU_Init())     {LED1 = !LED1;delay_ms(500),printf("MPU_Init error\r\n");}
	while(bmp280Init())   {LED2 = !LED2;delay_ms(500),printf("bmp280Init error\r\n");}
	while(NRF24L01_Init()){LED3 = !LED3;delay_ms(500),printf("NRF24L01_Init error\r\n");}
	NRF24L01_RX_Mode();	
	while(AT24CXX_Init()) {LED4 = !LED4;delay_ms(500),printf("AT24CXX_Init error\r\n");}
	printf("system ok\r\n");
//	FlashWriteData(0,  0.71,-0.26,-0.09,   1.0,1.01,0.99,   -23.47,23.86,10.03);//较为稳定数据2
	read_iic_eeprom();																 

	/*6、对频*/
	
	TIM3_Init();																	
	TIM4_PWM_Init();															
}

int main(void)
{
	system();
	while(1)
	{
		gyro_cal(&_gyro_orig_f);
		acc_cal(&_acc_att_lpf);
		VoltageDetection();
	}
}



