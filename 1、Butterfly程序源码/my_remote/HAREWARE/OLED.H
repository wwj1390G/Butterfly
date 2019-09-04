/*
CS：OLED片选信号。 

RST(RES)：硬复位OLED。 

DC：命令/数据标志（0，读写命令；1，读写数据）。 

SCLK：串行时钟线。在4线串行模式下，D0信号线作为串行时钟线SCLK。 

SDIN：串行数据线。在4线串行模式下，D1信号线作为串行数据线SDIN。

*/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "STM32F10x.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//使用4线串行接口时使用（SPI） 
#define OLED_SCK   PAout(8)  //D0   时钟线
#define OLED_SDA   PAout(9)  //D1   数据线
#define OLED_RST   PAout(10)  //RST  复位  
#define OLED_DC    PAout(11)  //RS   数据/命令  DC

#define OLED_CS    PAout(12) //片选


extern u8 OLED_Refresh;


void OLED_WR_Byte(u8 dat,u8 cmd);//写 命令/数据	 




//OLED控制用函数
void OLED_Init(void);//OLED初始化

void OLED_Photo(void); //图片显示函数

void OLED_Clear(void);//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!

void OLED_Refresh_Gram(void);//更新显存到LCD	
    
void OLED_Display_On(void);//开启OLED显示
    
void OLED_Display_Off(void);//关闭OLED显示 

/*  注：
	在一个循环中调用以下函数，都必须在函数后加更新显示函数：OLED_Refresh_Gram();
	当在一个循环调用多个以下函数时，只需在最后加 一次 更新显示函数：OLED_Refresh_Gram();      */



void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);//矩形填充

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//在指定位置显示一个字符

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);//显示字符串

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode);//显示数字



	 
#endif  
	 







 

