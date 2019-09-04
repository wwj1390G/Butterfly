#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
  	 
//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP


#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����2
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//��ȡ����4 
 

//#define KEY0_PRES 	1	//KEY0����
//#define KEY1_PRES	2	//KEY1����
//#define KEY2_PRES	3	//KEY2����
//#define KEY3_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��	
void check_key(void);
void Change_Mode(void);

typedef struct
{
	unsigned char left1;
	unsigned char left2;
	unsigned char right1;
	unsigned char right2;
}_key_;

extern _key_ _key;
#endif


