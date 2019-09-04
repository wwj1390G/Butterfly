#include "APP.h"
#include "uart1.h"
#include "delay.h"
#include "pid.h"
#include "led.h"
#include "pair_freq.h"
#include "key.h"
u8 data_to_send[50];	//发送数据缓存



/*
 *发送姿态数据
 *angle_rol：rol
 *angle_pit：pit
 *angle_yaw：Yaw
 *alt：		高度
 *fly_model：飞行模式
 *armed：0枷锁；1解锁
 */
static void ANO_DT_Send_Status(float angle_rol,float angle_pit,float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	
	vs32 _temp2 = alt*100;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	u8 sum = 0;
	for(u8 i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	DMA_SendData(data_to_send, _cnt);
}

/*
 *发送原始9轴数据
 * a_ ：加速度计数据
 * g_ ：陀螺仪数据
 * m_ ：磁力计数据
 */
static void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
	u8 _cnt=0,sum=0,i;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	//////////////////////////////////////////////////////////
	
	DMA_SendData(data_to_send,_cnt);
}

/*气压数据和超声波数据
 *bar：BMP280测得的海拔
 *csb：超声波测得的距离
 */
static void ANO_DT_Send_Senser2(s32 bar,u16 csb)
{
	u8 _cnt=0,sum=0,i;
	vs32 _temp2 = bar;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++]=BYTE1(csb);
	data_to_send[_cnt++]=BYTE0(csb);
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	//////////////////////////////////////////////////////////
	
	DMA_SendData(data_to_send,_cnt);
}

//***********************************************
//遥控器传来的数据
//***********************************************
static void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)
{
	u8 _cnt=0,sum=0,i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	//////////////////////////////////////////////////////////
	
	DMA_SendData(data_to_send,_cnt);
}

/*
 *发送PID数据
 *group：PID数据帧(1-6)
 */
static void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 _cnt=0,sum=0,i;
	vs16 _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = (int)(p1_p * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p1_i * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p1_d * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p2_p * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p2_i * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p2_d * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p3_p * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p3_i * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(p3_d * 1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	//////////////////////////////////////////////////////////
	
	DMA_SendData(data_to_send,_cnt);
}

/*
 *校验返回 ACK
 *head:帧头
 *check_sum：校验
 */
static void ANO_DT_Send_CHECK(u8 head, u8 check_sum)
{
	u8 _cnt=0,sum=0,i;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xEF;
	data_to_send[_cnt++]=0x00;
	
	
	data_to_send[_cnt++]=head;
	data_to_send[_cnt++]=check_sum;
  
	data_to_send[3] = _cnt-4;//获得数据长度

	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	//////////////////////////////////////////////////////////
	
	DMA_SendData(data_to_send,_cnt);
}



//**************************************
//命令集合1
//**************************************
static void CMD1(uint8_t data)
{
	switch(data)
	{
//		/*ACC校准*/
//		case 0x01:MPU_Get_Accel(	& mpu9250.acc.x,   & mpu9250.acc.y,  & mpu9250.acc.z,
//									&mpu_9250.acc.x,   &mpu_9250.acc.y,  &mpu_9250.acc.z);	//获得加速度计数据
//				break;
//		/*GYRO校准*/
//		case 0x02:MPU_Get_Gyro (	& mpu9250.gyr.x,   & mpu9250.gyr.y,  & mpu9250.gyr.z,
//									&mpu_9250.gyr.x,   &mpu_9250.gyr.y,  &mpu_9250.gyr.z);	//获得陀螺仪数据
//				break;
//		/*MAG校准*/
//		case 0x04:MPU_Get_Mag  (	& mpu9250.mag.x,   & mpu9250.mag.y,  & mpu9250.mag.z, 
//									&mpu_9250.mag.x,   &mpu_9250.mag.y,  &mpu_9250.mag.z);	//获得磁力计数据
//				break;
//		/*BARO校准*/
//		case 0x05:bmp280GetData(	&BMP_Data.pressure,&BMP_Data.height, &BMP_Data.temp);	//读取BMP的值	
//				break;
	}
}

//**************************************
//命令集合2
//**************************************
static void CMD2(uint8_t data)
{
	switch(data)
	{
		case 0x01:/*PID_Read_Flash();*/
							
							//外环 （角度环）
							ANO_DT_Send_PID(1,_PID_Angle.Roll.kp,  _PID_Angle.Roll.ki,  _PID_Angle.Roll.kd,
																_PID_Angle.Pitch.kp, _PID_Angle.Pitch.ki, _PID_Angle.Pitch.kd,
																_PID_Angle.Yaw.kp,   _PID_Angle.Yaw.ki,   _PID_Angle.Yaw.kd);delay_ms(2);
							//内环 （角速度环）
							ANO_DT_Send_PID(2,_PID_Rate.Roll.kp,  _PID_Rate.Roll.ki,  _PID_Rate.Roll.kd,
																_PID_Rate.Pitch.kp, _PID_Rate.Pitch.ki, _PID_Rate.Pitch.kd,
																_PID_Rate.Yaw.kp,   _PID_Rate.Yaw.ki,   _PID_Rate.Yaw.kd);delay_ms(2);
/*
							ANO_DT_Send_PID(3,0, 0, 0,
																0, 0, 0,
																0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(4,0, 0, 0,
																0, 0, 0,
																0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(5,0, 0, 0,
																0, 0, 0,
																0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(6,0, 0, 0,
																0, 0, 0,
																0, 0, 0);delay_ms(2);
*/
							break;
														  
									
	}
}

//**************************************
//上位机写入PID1
//**************************************
static void ANO_Write_PID1(uint8_t *rx)//PID 外环(角度环)
{
  _PID_Angle.Roll.kp = 0.001f*((rx[4]<<8)|rx[5]);
  _PID_Angle.Roll.ki = 0.001f*((rx[6]<<8)|rx[7]);
  _PID_Angle.Roll.kd = 0.001f*((rx[8]<<8)|rx[9]);
  
  _PID_Angle.Pitch.kp = 0.001f*((rx[10]<<8)|rx[11]);
  _PID_Angle.Pitch.ki = 0.001f*((rx[12]<<8)|rx[13]);
  _PID_Angle.Pitch.kd = 0.001f*((rx[14]<<8)|rx[15]);
  
  _PID_Angle.Yaw.kp = 0.001f*((rx[16]<<8)|rx[17]);
  _PID_Angle.Yaw.ki = 0.001f*((rx[18]<<8)|rx[19]);
  _PID_Angle.Yaw.kd = 0.001f*((rx[20]<<8)|rx[21]);
  
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}

//**************************************
//上位机写入PID2
//**************************************
static void ANO_Write_PID2(uint8_t *rx)//PID 内环(速率环)
{
  _PID_Rate.Roll.kp = 0.001f*((rx[4]<<8)|rx[5]);
  _PID_Rate.Roll.ki = 0.001f*((rx[6]<<8)|rx[7]);
  _PID_Rate.Roll.kd = 0.001f*((rx[8]<<8)|rx[9]);
  
  _PID_Rate.Pitch.kp = 0.001f*((rx[10]<<8)|rx[11]);
  _PID_Rate.Pitch.ki = 0.001f*((rx[12]<<8)|rx[13]);
  _PID_Rate.Pitch.kd = 0.001f*((rx[14]<<8)|rx[15]);
  
  _PID_Rate.Yaw.kp = 0.001f*((rx[16]<<8)|rx[17]);
  _PID_Rate.Yaw.ki = 0.001f*((rx[18]<<8)|rx[19]);
  _PID_Rate.Yaw.kd = 0.001f*((rx[20]<<8)|rx[21]);
  
  ANO_DT_Send_CHECK(rx[2],rx[22]);//返回 ACK
}

//**************************************
//上位机写入PID3
//**************************************
static void ANO_Write_PID3(uint8_t *rx)//高度PID
{
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}

//**************************************
//上位机写入PID4
//**************************************
static void ANO_Write_PID4(uint8_t *rx)
{
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}

//**************************************
//上位机写入PID5
//**************************************
static void ANO_Write_PID5(uint8_t *rx)
{
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}

//**************************************
//上位机写入PID6
//**************************************
static void ANO_Write_PID6(uint8_t *rx)
{
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}


/*
 *向上位机发送数据
 */
static void ANO_DMA_SEND_DATA(void)
{
//	ANO_DT_Send_Status(att.rol,att.pit,att.yaw,0,0,0);//姿态数据
	
//	printf("%f   %f   %f\r\n",att.rol,att.pit,att.yaw);
	//发送9轴数据补偿值
//	ANO_DT_Send_Senser(	_acc_orig_f.x*100,_acc_orig_f.y*100,_acc_orig_f.z*100,
//											_gyro_orig_f.x*10,_gyro_orig_f.y*10,_gyro_orig_f.z*10,
//											0,0,0);//加速度计放大100倍，陀螺仪放大10倍，没有磁力计
}


//**************************************
//接收上位机的数据
//**************************************
static void ANO_DMA_READ_DATA(void)
{
	uint8_t usart_rx[32];
	
	if(USART_RX_FLAG==1)//接收串口传来的数据
	{
		LED1 = !LED1;
		//保存数据，防止数据在处理过程中被新数据覆盖
		for(uint8_t i=0;i<32;i++)
		usart_rx[i]=RxBuff[i];
		
		//解析
		if((usart_rx[0]==0XAA)&&(usart_rx[1]==0XAF))
		{
			switch(usart_rx[2])
			{
				case 0x01:CMD1(usart_rx[4]);break;
				case 0x02:CMD2(usart_rx[4]);break;
				case 0x10:ANO_Write_PID1(usart_rx);break;
				case 0x11:ANO_Write_PID2(usart_rx);break;
				case 0x12:ANO_Write_PID3(usart_rx);break;
				case 0x13:ANO_Write_PID4(usart_rx);break;
				case 0x14:ANO_Write_PID5(usart_rx);break;
				case 0x15:ANO_Write_PID6(usart_rx);/*PID_Write_Flash()*/;break;
			}
		}
		USART_RX_FLAG = 0;
	}
}

void ANO_DMA_DATA(void)
{
//	ANO_DMA_SEND_DATA();
	ANO_DT_Send_Status(pair.com_mode,pair.fly_mode,0,0,0,0);
	ANO_DMA_READ_DATA();
}
