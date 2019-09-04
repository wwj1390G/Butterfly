#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "calm_mpu.h"

_Mpu6050_data _Mpu = {0};

_S16xyz _gyro_orig = {0};//陀螺仪 原始数据
_F32xyz _gyro_offset = {0};//陀螺仪 偏移数据
_F32xyz _gyro_orig_f = {0};//偏移校准后的数据


_S16xyz _acc_orig = {0};//加速度计 原始数据
_F32xyz _acc_orig_f = {0};//偏移校准后的数据
_F32xyz _acc_att_lpf = {0};//滤波后的值

_F32xyz _acc_fix_lpf = {0};//高度滤波后的值

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);//IIC连续写
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); //IIC连续读 
u8 MPU_Write_Byte(u8 reg,u8 data);				//IIC写一个字节
u8 MPU_Read_Byte(u8 reg);						//IIC读一个字节

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);

//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
u8 MPU_Init(void)
{ 
	u8 res;
/*	
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//先使能外设IO PORTA时钟 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 // 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!
	
	MPU_AD0_CTRL=0;			//控制MPU6050的AD0脚为低电平,从机地址为:0X68
*/	
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(2);					//加速度传感器,±8g
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X82);	//INT引脚低电平有效，处理器可以访问辅助IIC总线
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
		float temp;
		MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
  return res;;
}

//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//发送数据
		if(MPU_IIC_Wait_Ack())		//等待ACK
		{
			MPU_IIC_Stop();	 
			return 1;		 
		}		
	}    
    MPU_IIC_Stop();	 
	return 0;	
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
    MPU_IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    MPU_IIC_Stop();	//产生一个停止条件 
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(MPU_IIC_Wait_Ack())	//等待应答
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答 
	MPU_IIC_Send_Byte(data);//发送数据
	if(MPU_IIC_Wait_Ack())	//等待ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}		 
    MPU_IIC_Stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	MPU_IIC_Wait_Ack();		//等待应答 
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
    MPU_IIC_Wait_Ack();		//等待应答 
	res=MPU_IIC_Read_Byte(0);//读取数据,发送nACK 
    MPU_IIC_Stop();			//产生一个停止条件 
	return res;		
}



/*************************************************************************/
//求IIR滤波因子
void get_iir_factor(float *out_factor,float Time, float Cut_Off)
{
	*out_factor = Time /( Time + 1/(2.0f * PI * Cut_Off) );
}

//IIR低通滤波器
void acc_iir_lpf(_F32xyz *acc_in,_F32xyz *acc_out,float lpf_factor)
{
	acc_out->x = acc_out->x + lpf_factor*(acc_in->x - acc_out->x); 
	acc_out->y = acc_out->y + lpf_factor*(acc_in->y - acc_out->y); 
	acc_out->z = acc_out->z + lpf_factor*(acc_in->z - acc_out->z); 
}

//加速度计滤波参数
_Butterworth_parameter acc_5hz_parameter =
{
	/*200hz---1hz*/
//  1,   -1.955578240315,   0.9565436765112,
//  0.000241359049042, 0.000482718098084, 0.000241359049042
	/*200hz---2hz*/
//  1,   -1.911197067426,   0.9149758348014,
//  0.0009446918438402,  0.00188938368768,0.0009446918438402
	/*200hz---5hz*/
    1,                  -1.778631777825,    0.8008026466657,
    0.005542717210281,   0.01108543442056,  0.005542717210281
	/*200hz---10hz*/
//    1,   -1.561018075801,   0.6413515380576,
//    0.02008336556421,  0.04016673112842,  0.02008336556421
    /*200hz---15hz*/
//    1,   -1.348967745253,   0.5139818942197,
//    0.04125353724172,  0.08250707448344,  0.04125353724172
    /*200hz---20hz*/
//    1,    -1.14298050254,   0.4128015980962,
//    0.06745527388907,   0.1349105477781,  0.06745527388907
    /*200hz---30hz*/
//    1,  -0.7477891782585,    0.272214937925,
//    0.1311064399166,   0.2622128798333,   0.1311064399166 
}; 

_Butterworth_data   acc_butter_data[3];
//加速度计巴特沃斯低通滤波
void acc_butterworth_lpf(_F32xyz *acc_in,_F32xyz *acc_out)
{
    acc_out->x = butterworth_lpf(acc_in->x,&acc_butter_data[0],&acc_5hz_parameter);
    acc_out->y = butterworth_lpf(acc_in->y,&acc_butter_data[1],&acc_5hz_parameter);
    acc_out->z = butterworth_lpf(acc_in->z,&acc_butter_data[2],&acc_5hz_parameter);    
}


//二阶butterworth滤波器-lpf
float butterworth_lpf(float now_input,_Butterworth_data *buffer, _Butterworth_parameter *parameter)
{
    buffer->input_data[2] = now_input;

    /* Butterworth LPF */
    buffer->output_data[2] =   parameter->b[0] * buffer->input_data[2]
                             + parameter->b[1] * buffer->input_data[1]
                             + parameter->b[2] * buffer->input_data[0]
                             - parameter->a[1] * buffer->output_data[1]
                             - parameter->a[2] * buffer->output_data[0];
    /* x(n) 保存 */
    buffer->input_data[0] = buffer->input_data[1];
    buffer->input_data[1] = buffer->input_data[2];
    /* y(n) 保存 */
    buffer->output_data[0] = buffer->output_data[1];
    buffer->output_data[1] = buffer->output_data[2];
    
    return buffer->output_data[2];
}
//原始加速度量转化为 g
void get_acc_g(_F32xyz *acc_in,_F32xyz *acc_out)
{
		acc_out->x = (float)(acc_in->x * acc_raw_to_g);
		acc_out->y = (float)(acc_in->y * acc_raw_to_g);
		acc_out->z = (float)(acc_in->z * acc_raw_to_g);
}

//raw -> rad/s	转/秒
void get_rad_s(_F32xyz *gyro_in,_F32xyz *gyro_out)
{
	gyro_out->x = (float)(gyro_in->x * gyro_raw_to_radian_s);
	gyro_out->y = (float)(gyro_in->y * gyro_raw_to_radian_s);
	gyro_out->z = (float)(gyro_in->z * gyro_raw_to_radian_s);
}

//raw -> deg/s  度/秒
void get_deg_s(_F32xyz *gyro_in,_F32xyz *gyro_deg_out)
{
	gyro_deg_out->x = (float)(gyro_in->x * gyro_raw_to_deg_s);
	gyro_deg_out->y = (float)(gyro_in->y * gyro_raw_to_deg_s);
	gyro_deg_out->z = (float)(gyro_in->z * gyro_raw_to_deg_s);    
}
/******************************************************************/


_Butterworth_parameter gyro_30hz_parameter =
{
    //200hz---30hz
    1,  -0.7477891782585,    0.272214937925,
    0.1311064399166,   0.2622128798333,   0.1311064399166 
}; 

_Butterworth_data   gyro_butter_data[3];

/*对陀螺仪原始数据进行偏零校准
 */
void get_gyro_raw(void)
{
	MPU_Get_Gyroscope(&_gyro_orig.x,&_gyro_orig.y,&_gyro_orig.z);
	
	_gyro_orig.x = _gyro_orig.x - _gyro_offset.x;
	_gyro_orig.y = _gyro_orig.y - _gyro_offset.y;
	_gyro_orig.z = _gyro_orig.z - _gyro_offset.z;

	_gyro_orig_f.x = (float)butterworth_lpf(((float)_gyro_orig.x),&gyro_butter_data[0],&gyro_30hz_parameter);
	_gyro_orig_f.y = (float)butterworth_lpf(((float)_gyro_orig.y),&gyro_butter_data[1],&gyro_30hz_parameter);
	_gyro_orig_f.z = (float)butterworth_lpf(((float)_gyro_orig.z),&gyro_butter_data[2],&gyro_30hz_parameter);
}

/*加速度计校准
 */
void get_acc_raw(void)
{
	MPU_Get_Accelerometer(&_acc_orig.x,&_acc_orig.y,&_acc_orig.z);

	_acc_orig_f.x = (float)(_acc_cal.K[0]*((float)_acc_orig.x) - _acc_cal.B[0]*one_g_to_acc);
	_acc_orig_f.y = (float)(_acc_cal.K[1]*((float)_acc_orig.y) - _acc_cal.B[1]*one_g_to_acc);
	_acc_orig_f.z = (float)(_acc_cal.K[2]*((float)_acc_orig.z) - _acc_cal.B[2]*one_g_to_acc);

}











