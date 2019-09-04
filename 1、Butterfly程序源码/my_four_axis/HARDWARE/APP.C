#include "APP.h"
#include "uart2.h"
#include "delay.h"
#include "imu.h"
#include "mpu6050.h"
#include "NRFCommunication.h"
#include "pid.h"
/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义,在发送大于1字节的数据类型时,比如int16、float等,需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

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
	vs32 _temp1 = bar*100;
	u32  _temp2 = csb*100;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE3(_temp1);
	data_to_send[_cnt++]=BYTE2(_temp1);
	data_to_send[_cnt++]=BYTE1(_temp1);
	data_to_send[_cnt++]=BYTE0(_temp1);
	
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
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
							/*发送外环 角度环*/
							ANO_DT_Send_PID(1,all.rol_angle.kp, all.rol_angle.ki, all.rol_angle.kd,
																all.pit_angle.kp, all.pit_angle.ki, all.pit_angle.kd,
																all.yaw_angle.kp, all.yaw_angle.ki, all.yaw_angle.kd);delay_ms(2);	
							/*发送内环 角速度环*/
							ANO_DT_Send_PID(2,all.rol_gyro.kp, all.rol_gyro.ki, all.rol_gyro.kd,
																all.pit_gyro.kp, all.pit_gyro.ki, all.pit_gyro.kd,
																all.yaw_gyro.kp, all.yaw_gyro.ki, all.yaw_gyro.kd);delay_ms(2);	
/*
							ANO_DT_Send_PID(3, 0, 0, 0,
											0, 0, 0,
											0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(4, 0, 0, 0,
											0, 0, 0,
											0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(5, 0, 0, 0,
											0, 0, 0,
											0, 0, 0);delay_ms(2);

							ANO_DT_Send_PID(6, 0, 0, 0,
											0, 0, 0,
											0, 0, 0);
*/	
											break;
														  
							
	}
}


//**************************************
//上位机写入PID1
//**************************************
static void ANO_Write_PID1(uint8_t *rx)//PID 外环(角度环)
{
  all.rol_angle.kp = 0.001f*((rx[4]<<8)|rx[5]);   
  all.rol_angle.ki = 0.001f*((rx[6]<<8)|rx[7]);
  all.rol_angle.kd = 0.001f*((rx[8]<<8)|rx[9]);
  
  all.pit_angle.kp = 0.001f*((rx[10]<<8)|rx[11]);
  all.pit_angle.ki = 0.001f*((rx[12]<<8)|rx[13]);
  all.pit_angle.kd = 0.001f*((rx[14]<<8)|rx[15]);
  
  all.yaw_angle.kp = 0.001f*((rx[16]<<8)|rx[17]);
  all.yaw_angle.ki = 0.001f*((rx[18]<<8)|rx[19]);
  all.yaw_angle.kd = 0.001f*((rx[20]<<8)|rx[21]);
  
  ANO_DT_Send_CHECK(rx[2],rx[22]);
}

//**************************************
//上位机写入PID2
//**************************************
static void ANO_Write_PID2(uint8_t *rx)//PID 内环(速率环)
{
  all.rol_gyro.kp = 0.001f*((rx[4]<<8)|rx[5]);
  all.rol_gyro.ki = 0.001f*((rx[6]<<8)|rx[7]);
  all.rol_gyro.kd = 0.001f*((rx[8]<<8)|rx[9]);
  
  all.pit_gyro.kp = 0.001f*((rx[10]<<8)|rx[11]);
  all.pit_gyro.ki = 0.001f*((rx[12]<<8)|rx[13]);
  all.pit_gyro.kd = 0.001f*((rx[14]<<8)|rx[15]);
  
  all.yaw_gyro.kp = 0.001f*((rx[16]<<8)|rx[17]);
  all.yaw_gyro.ki = 0.001f*((rx[18]<<8)|rx[19]);
  all.yaw_gyro.kd = 0.001f*((rx[20]<<8)|rx[21]);
  
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


//**************************************
//接收上位机的数据命令
//**************************************
static void ANO_DMA_READ_DATA(void)
{
	uint8_t usart_rx[32];
	
	if(USART_RX_FLAG==1)//接收串口传来的数据
	{
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
	static u8 num = 0;
	num ++;
	if(num%2 == 0)
	{
		ANO_DT_Send_Status(att.rol,att.pit,att.yaw,0,0,0);//发送 姿态数据
	}
	
	
//	ANO_DT_Send_RCData(_RC.thr,_RC.yaw,_RC.rol,_RC.pit,11,22,33,44,55,66);//发送 遥控器数据
	
	
	if(num%4 == 0)
	{
//		ANO_DT_Send_Senser(_acc_orig.x,_acc_orig.y,_acc_orig.z,_gyro_orig.x,_gyro_orig.y,_gyro_orig.z,0,0,0);//发送 加速度计/陀螺仪/磁力计 数据
			ANO_DMA_READ_DATA();//接收 上位机 指令
	}
	
//	ANO_DT_Send_Senser2(150,200);//发送 海拔/高度数据
	
	if(num>100)
	{
		num = 0;
	}
}



