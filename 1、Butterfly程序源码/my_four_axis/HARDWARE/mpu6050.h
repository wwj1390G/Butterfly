#ifndef __MPU6050_H
#define __MPU6050_H
#include "mpuiic.h"   												  	  
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEKÕ½½¢STM32¿ª·¢°åV3
//MPU6050 Çı¶¯´úÂë	   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2015/1/17
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
//MPU6050 AD0¿ØÖÆ½Å
//#define MPU_AD0_CTRL			PAout(15)	//¿ØÖÆAD0µçÆ½,´Ó¶ø¿ØÖÆMPUµØÖ·

//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs¼Ä´æÆ÷,¿É¶ÁÈ¡°æ±¾ºÅ,¼Ä´æÆ÷ÊÖ²áÎ´Ìáµ½
//#define MPU_PROD_ID_REG			0X0C	//prod id¼Ä´æÆ÷,ÔÚ¼Ä´æÆ÷ÊÖ²áÎ´Ìáµ½
#define MPU_SELF_TESTX_REG		0X0D	//×Ô¼ì¼Ä´æÆ÷X
#define MPU_SELF_TESTY_REG		0X0E	//×Ô¼ì¼Ä´æÆ÷Y
#define MPU_SELF_TESTZ_REG		0X0F	//×Ô¼ì¼Ä´æÆ÷Z
#define MPU_SELF_TESTA_REG		0X10	//×Ô¼ì¼Ä´æÆ÷A
#define MPU_SAMPLE_RATE_REG		0X19	//²ÉÑùÆµÂÊ·ÖÆµÆ÷
#define MPU_CFG_REG				0X1A	//ÅäÖÃ¼Ä´æÆ÷
#define MPU_GYRO_CFG_REG		0X1B	//ÍÓÂİÒÇÅäÖÃ¼Ä´æÆ÷
#define MPU_ACCEL_CFG_REG		0X1C	//¼ÓËÙ¶È¼ÆÅäÖÃ¼Ä´æÆ÷
#define MPU_MOTION_DET_REG		0X1F	//ÔË¶¯¼ì²â·§ÖµÉèÖÃ¼Ä´æÆ÷
#define MPU_FIFO_EN_REG			0X23	//FIFOÊ¹ÄÜ¼Ä´æÆ÷
#define MPU_I2CMST_CTRL_REG		0X24	//IICÖ÷»ú¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC´Ó»ú0Æ÷¼şµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV0_REG			0X26	//IIC´Ó»ú0Êı¾İµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC´Ó»ú0¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC´Ó»ú1Æ÷¼şµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV1_REG			0X29	//IIC´Ó»ú1Êı¾İµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC´Ó»ú1¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC´Ó»ú2Æ÷¼şµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV2_REG			0X2C	//IIC´Ó»ú2Êı¾İµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC´Ó»ú2¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC´Ó»ú3Æ÷¼şµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV3_REG			0X2F	//IIC´Ó»ú3Êı¾İµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC´Ó»ú3¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC´Ó»ú4Æ÷¼şµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV4_REG			0X32	//IIC´Ó»ú4Êı¾İµØÖ·¼Ä´æÆ÷
#define MPU_I2CSLV4_DO_REG		0X33	//IIC´Ó»ú4Ğ´Êı¾İ¼Ä´æÆ÷
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC´Ó»ú4¿ØÖÆ¼Ä´æÆ÷
#define MPU_I2CSLV4_DI_REG		0X35	//IIC´Ó»ú4¶ÁÊı¾İ¼Ä´æÆ÷

#define MPU_I2CMST_STA_REG		0X36	//IICÖ÷»ú×´Ì¬¼Ä´æÆ÷
#define MPU_INTBP_CFG_REG		0X37	//ÖĞ¶Ï/ÅÔÂ·ÉèÖÃ¼Ä´æÆ÷
#define MPU_INT_EN_REG			0X38	//ÖĞ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
#define MPU_INT_STA_REG			0X3A	//ÖĞ¶Ï×´Ì¬¼Ä´æÆ÷

#define MPU_ACCEL_XOUTH_REG		0X3B	//¼ÓËÙ¶ÈÖµ,XÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_XOUTL_REG		0X3C	//¼ÓËÙ¶ÈÖµ,XÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_ACCEL_YOUTH_REG		0X3D	//¼ÓËÙ¶ÈÖµ,YÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_YOUTL_REG		0X3E	//¼ÓËÙ¶ÈÖµ,YÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_ACCEL_ZOUTH_REG		0X3F	//¼ÓËÙ¶ÈÖµ,ZÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_ACCEL_ZOUTL_REG		0X40	//¼ÓËÙ¶ÈÖµ,ZÖáµÍ8Î»¼Ä´æÆ÷

#define MPU_TEMP_OUTH_REG		0X41	//ÎÂ¶ÈÖµ¸ß°ËÎ»¼Ä´æÆ÷
#define MPU_TEMP_OUTL_REG		0X42	//ÎÂ¶ÈÖµµÍ8Î»¼Ä´æÆ÷

#define MPU_GYRO_XOUTH_REG		0X43	//ÍÓÂİÒÇÖµ,XÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_XOUTL_REG		0X44	//ÍÓÂİÒÇÖµ,XÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_GYRO_YOUTH_REG		0X45	//ÍÓÂİÒÇÖµ,YÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_YOUTL_REG		0X46	//ÍÓÂİÒÇÖµ,YÖáµÍ8Î»¼Ä´æÆ÷
#define MPU_GYRO_ZOUTH_REG		0X47	//ÍÓÂİÒÇÖµ,ZÖá¸ß8Î»¼Ä´æÆ÷
#define MPU_GYRO_ZOUTL_REG		0X48	//ÍÓÂİÒÇÖµ,ZÖáµÍ8Î»¼Ä´æÆ÷

#define MPU_I2CSLV0_DO_REG		0X63	//IIC´Ó»ú0Êı¾İ¼Ä´æÆ÷
#define MPU_I2CSLV1_DO_REG		0X64	//IIC´Ó»ú1Êı¾İ¼Ä´æÆ÷
#define MPU_I2CSLV2_DO_REG		0X65	//IIC´Ó»ú2Êı¾İ¼Ä´æÆ÷
#define MPU_I2CSLV3_DO_REG		0X66	//IIC´Ó»ú3Êı¾İ¼Ä´æÆ÷

#define MPU_I2CMST_DELAY_REG	0X67	//IICÖ÷»úÑÓÊ±¹ÜÀí¼Ä´æÆ÷
#define MPU_SIGPATH_RST_REG		0X68	//ĞÅºÅÍ¨µÀ¸´Î»¼Ä´æÆ÷
#define MPU_MDETECT_CTRL_REG	0X69	//ÔË¶¯¼ì²â¿ØÖÆ¼Ä´æÆ÷
#define MPU_USER_CTRL_REG		0X6A	//ÓÃ»§¿ØÖÆ¼Ä´æÆ÷
#define MPU_PWR_MGMT1_REG		0X6B	//µçÔ´¹ÜÀí¼Ä´æÆ÷1
#define MPU_PWR_MGMT2_REG		0X6C	//µçÔ´¹ÜÀí¼Ä´æÆ÷2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO¼ÆÊı¼Ä´æÆ÷¸ß°ËÎ»
#define MPU_FIFO_CNTL_REG		0X73	//FIFO¼ÆÊı¼Ä´æÆ÷µÍ°ËÎ»
#define MPU_FIFO_RW_REG			0X74	//FIFO¶ÁĞ´¼Ä´æÆ÷
#define MPU_DEVICE_ID_REG		0X75	//Æ÷¼şID¼Ä´æÆ÷
 
//Èç¹ûAD0½Å(9½Å)½ÓµØ,IICµØÖ·Îª0X68(²»°üº¬×îµÍÎ»).
//Èç¹û½ÓV3.3,ÔòIICµØÖ·Îª0X69(²»°üº¬×îµÍÎ»).
#define MPU_ADDR				0X68


////ÒòÎªÄ£¿éAD0Ä¬ÈÏ½ÓGND,ËùÒÔ×ªÎª¶ÁĞ´µØÖ·ºó,Îª0XD1ºÍ0XD0(Èç¹û½ÓVCC,ÔòÎª0XD3ºÍ0XD2)  
//#define MPU_READ    0XD1
//#define MPU_WRITE   0XD0

u8 MPU_Init(void); 								//³õÊ¼»¯MPU6050
short MPU_Get_Temperature(void);//µÃµ½ÎÂ¶ÈÖµ(À©´óÁË100±¶)
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);//µÃµ½ÍÓÂİÒÇÖµ(Ô­Ê¼Öµ)
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);//µÃµ½¼ÓËÙ¶ÈÖµ(Ô­Ê¼Öµ)


/*****************************************/

#define PI                      3.1415926535898f
#define gyro_raw_to_deg_s       0.0609756097561f   //+-250°/s:131LSB/°/s   +-500°/s:65.5LSB/°/s   +-1000°/s:32.8LSB/°/s    +-2000°/s:16.4LSB/°/s(????)
#define acc_raw_to_g            0.000244140625f    //+-2g : 16384LSB/g     +-4g : 8192LSB/g   +-8g : 4096LSB/g(????)   +-16g : 2048LSB/g  

#define deg_to_rad              (PI / 180.0f)
#define rad_to_angle            (180.0f / PI)                    
#define gyro_raw_to_radian_s	(gyro_raw_to_deg_s * deg_to_rad)

#define accmax_1g      4096
#define gravity_mss    9.80665f//ÖØÁ¦¼ÓËÙ¶È(m/s/s)   acceleration due to gravity in m/s/s

#define acc_to_1g      gravity_mss / accmax_1g
#define one_g_to_acc   accmax_1g / gravity_mss

typedef struct
{
	short x;
	short y;
	short z;
}_S16xyz;

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
}U16_XYZ;


typedef struct
{
	float x;
	float y;
	float z;
}_F32xyz;

typedef struct 
{
	_F32xyz deg_s;
	_F32xyz rad_s;
	_F32xyz acc_g;

	float Acc_att_coe;//×ËÌ¬ ¼ÓËÙ¶È µÍÍ¨ÂË²¨ÏµÊı 
  float Acc_alt_coe;//¸ß¶È ¼ÓËÙ¶È µÍÍ¨ÂË²¨ÏµÊı 
  
}_Mpu6050_data;




typedef struct
{
    float input_data[3];
    float output_data[3];
}_Butterworth_data;



typedef struct
{
    const float a[3];
    const float b[3];
}_Butterworth_parameter;


float butterworth_lpf(float now_input,_Butterworth_data *buffer, _Butterworth_parameter *parameter);//¶ş½×butterworthÂË²¨Æ÷
void get_iir_factor(float *out_factor,float Time, float Cut_Off);//ÇóIIRÂË²¨Òò×Ó
void acc_iir_lpf(_F32xyz *acc_in,_F32xyz *acc_out,float lpf_factor);//IIRµÍÍ¨ÂË²¨Æ÷
void acc_butterworth_lpf(_F32xyz *acc_in,_F32xyz *acc_out);//¼ÓËÙ¶È¼Æ°ÍÌØÎÖË¹µÍÍ¨ÂË²¨

void get_acc_g(_F32xyz *acc_in,_F32xyz *acc_out);//Ô­Ê¼¼ÓËÙ¶ÈÁ¿×ª»¯Îª g
void get_rad_s(_F32xyz *gyro_in,_F32xyz *gyro_out);//raw -> rad/s	×ª/Ãë
void get_deg_s(_F32xyz *gyro_in,_F32xyz *gyro_deg_out);//raw -> deg/s  ¶È/Ãë
void get_gyro_raw(void);//µÃµ½ÍÓÂİÒÇÊı¾İÆ«ÒÆĞ£×¼Öµ
void get_acc_raw(void);//µÃµ½¼ÓËÙ¶È¼ÆĞ£×¼Öµ

extern _S16xyz _gyro_orig;//ÍÓÂİÒÇ Ô­Ê¼Êı¾İ
extern _Mpu6050_data _Mpu;
extern _F32xyz _gyro_orig_f;
extern _F32xyz _gyro_offset;//ÍÓÂİÒÇ Æ«ÒÆÊı¾İ

extern _S16xyz _acc_orig;//¼ÓËÙ¶È¼Æ Ô­Ê¼Êı¾İ
extern _F32xyz _acc_orig_f;
extern _F32xyz _acc_att_lpf;
extern _F32xyz _acc_fix_lpf;
#endif




































