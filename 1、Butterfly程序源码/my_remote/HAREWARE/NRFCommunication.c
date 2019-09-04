#include "NRFCommunication.h"
#include "NRF24L01.h"
#include "adc.h"
#include "pair_freq.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "pid.h"
#include "APP.h"
_RC_Data _RC = {0};

uint8_t tx_dat[TX_PLOAD_WIDTH] = {0};

//数据转换
static void DataConversion(unsigned short int* adc_dat)
{
	//油门数据范围转换为0~1000
	//打舵数据范围转换为0~100
    _RC.thr = (uint16_t)(1000.0f - ((float)(adc_dat[2]))*0.24420f);
    
    _RC.pit = (uint8_t)(((float)(adc_dat[1]))*0.02442f);
    _RC.rol = (uint8_t)(((float)(adc_dat[0]))*0.02442f);
	
		_RC.yaw = (uint8_t)(((float)(adc_dat[3]))*0.02442f);
}

//整合数据
static void IntegratedData(uint8_t *dat_buf)
{
		int16_t _temp;
    //对频数据
    if(pair.com_mode == 1)
    {
//        dat_buf[0] = 0xA1;//帧头
//        dat_buf[1] = pair.addr[0];		  //5?????    
//        dat_buf[2] = pair.addr[1];	    //    
//        dat_buf[3] = pair.addr[2];			//	
//        dat_buf[4] = pair.addr[3];			//	
//        dat_buf[5] = pair.addr[4];			//	
//        
//        dat_buf[6] = pair.freq_channel[0];		//4?????		
//        dat_buf[7] = pair.freq_channel[1];				
//        dat_buf[8] = pair.freq_channel[2];                         
//        dat_buf[9] = pair.freq_channel[3];

//        dat_buf[TX_PLOAD_WIDTH-1] = 0x1A;	//帧尾     
    }
		//正常通信数据
    else if(pair.com_mode == 2)
    {
        dat_buf[0] = 0xB2;//帧头
        dat_buf[1] = *((u8*)&_RC.thr);//油门低八位
        dat_buf[2] = *(((u8*)&_RC.thr)+1);	//油门高八位
        dat_buf[3] = _RC.pit;
        dat_buf[4] = _RC.rol;
        dat_buf[5] = _RC.yaw;
        dat_buf[6] = _key.left1;
        dat_buf[7] = _key.right1;
				dat_buf[8] = pair.fly_mode;
				dat_buf[TX_PLOAD_WIDTH-1] = 0x2B;//帧尾     
    }
		//PID参数调节   外环（角度）调试数据
		else if(pair.com_mode == 3)
		{
				dat_buf[0] = 0xC3;//帧头
				dat_buf[1] = *((u8*)&_RC.thr);//油门低八位
				dat_buf[2] = *(((u8*)&_RC.thr)+1);	//油门高八位
		
				/*外环 角度*/
				_temp = (int)(_PID_Angle.Pitch.kp * 1000);
				dat_buf[3]  = BYTE1(_temp);
				dat_buf[4]  =	BYTE0(_temp);		
				_temp = (int)(_PID_Angle.Pitch.ki * 1000);
				dat_buf[5]  = BYTE1(_temp);
				dat_buf[6]  =	BYTE0(_temp);			
				_temp = (int)(_PID_Angle.Pitch.kd * 1000);
				dat_buf[7]  = BYTE1(_temp);
				dat_buf[8]  =	BYTE0(_temp);
			
				_temp = (int)(_PID_Angle.Roll.kp * 1000);
				dat_buf[9]   = BYTE1(_temp);
				dat_buf[10]  = BYTE0(_temp);		
				_temp = (int)(_PID_Angle.Roll.ki * 1000);
				dat_buf[11]  = BYTE1(_temp);
				dat_buf[12]  = BYTE0(_temp);			
				_temp = (int)(_PID_Angle.Roll.kd * 1000);
				dat_buf[13]  = BYTE1(_temp);
				dat_buf[14]  = BYTE0(_temp);
				
				_temp = (int)(_PID_Angle.Yaw.kp * 1000);
				dat_buf[15]  = BYTE1(_temp);
				dat_buf[16]  = BYTE0(_temp);		
				_temp = (int)(_PID_Angle.Yaw.ki * 1000);
				dat_buf[17]  = BYTE1(_temp);
				dat_buf[18]  = BYTE0(_temp);			
				_temp = (int)(_PID_Angle.Yaw.kd * 1000);
				dat_buf[19]  = BYTE1(_temp);
				dat_buf[20]  = BYTE0(_temp);
			
        dat_buf[TX_PLOAD_WIDTH-1] = 0x3C;//帧尾     
		}	
		//PID 内环（角速度）调试数据
		 else if(pair.com_mode == 4)
    {
				dat_buf[0] = 0xD4;//帧头
				dat_buf[1] = *((u8*)&_RC.thr);//油门低八位
				dat_buf[2] = *(((u8*)&_RC.thr)+1);	//油门高八位
			
				/*内环 角速度*/
				_temp = (int)(_PID_Rate.Pitch.kp * 1000);
				dat_buf[3]  = BYTE1(_temp);
				dat_buf[4]  =	BYTE0(_temp);		
				_temp = (int)(_PID_Rate.Pitch.ki * 1000);
				dat_buf[5]  = BYTE1(_temp);
				dat_buf[6]  =	BYTE0(_temp);			
				_temp = (int)(_PID_Rate.Pitch.kd * 1000);
				dat_buf[7]  = BYTE1(_temp);
				dat_buf[8]  =	BYTE0(_temp);

				_temp = (int)(_PID_Rate.Roll.kp * 1000);
				dat_buf[9]   = BYTE1(_temp);
				dat_buf[10]  = BYTE0(_temp);		
				_temp = (int)(_PID_Rate.Roll.ki * 1000);
				dat_buf[11]  = BYTE1(_temp);
				dat_buf[12]  = BYTE0(_temp);			
				_temp = (int)(_PID_Rate.Roll.kd * 1000);
				dat_buf[13]  = BYTE1(_temp);
				dat_buf[14]  = BYTE0(_temp);

				_temp = (int)(_PID_Rate.Yaw.kp * 1000);
				dat_buf[15]  = BYTE1(_temp);
				dat_buf[16]  = BYTE0(_temp);		
				_temp = (int)(_PID_Rate.Yaw.ki * 1000);
				dat_buf[17]  = BYTE1(_temp);
				dat_buf[18]  = BYTE0(_temp);			
				_temp = (int)(_PID_Rate.Yaw.kd * 1000);
				dat_buf[19]  = BYTE1(_temp);
				dat_buf[20]  = BYTE0(_temp);
				
				dat_buf[TX_PLOAD_WIDTH-1] = 0x4D;//帧尾    
		}
		
    
}
	
//DMA传输ADC数据完成中断，发送数据
void DMA1_Channel1_IRQHandler(void)
{
	unsigned char val;
	
	if(DMA_GetITStatus(DMA1_IT_TC1)==SET)
	{	
		DataConversion(adc_value);

		IntegratedData(tx_dat);//将要发送的数据整合，存入tx_dat

		//NRF发送数据
		val = NRF24L01_TxPacket(tx_dat);
		
		if(val == 0xff)//发送失败，可能是失联
		{
			if(_RC.lose_num < 200)
			{
				_RC.lose_num++;
				_RC.lose_flag = 0;
			}
			if(_RC.lose_num == 200)
			{
				_RC.lose_flag = 1;//失联标志位
			}
		}
		else
		{
			_RC.lose_num = 0;
		}		
		
		DMA_ClearITPendingBit(DMA1_IT_TC1);			
	}
	
}

