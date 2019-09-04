#ifndef __APP_H
#define __APP_H
#include "stm32F10x.h"
extern u8 data_to_send[50];	//发送数据缓存

/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义,在发送大于1字节的数据类型时,比如int16、float等,需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


//发送数据函数
//void ANO_DMA_SEND_DATA(void);//向上位机数据

//void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);//发送姿态数据
//void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);//发送原始9轴数据
//void ANO_DT_Send_Senser2(s32 bar,u16 csb);//气压数据和超声波数据
//void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);//遥控器传来的数据
//void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);//发送PID数据
//void ANO_DT_Send_CHECK(uint8_t head, uint8_t check_sum);//校验返回 ACK



//接收数据函数
//void ANO_DMA_READ_DATA(void);//解析串口数据

//void CMD1(uint8_t data);
//void CMD2(uint8_t data);
//void ANO_Write_PID1(uint8_t *rx);
//void ANO_Write_PID2(uint8_t *rx);
//void ANO_Write_PID3(uint8_t *rx);
//void ANO_Write_PID4(uint8_t *rx);
//void ANO_Write_PID5(uint8_t *rx);
//void ANO_Write_PID6(uint8_t *rx);

void ANO_DMA_DATA(void);
#endif

