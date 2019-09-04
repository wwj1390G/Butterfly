#ifndef __QUERY_TAST_H
#define __QUERY_TAST_H
#include "stm32f10x.h"

void QueryTast(void);

typedef struct
{
	uint8_t lock_flag   : 2 ;//上锁标志位   0:无操作  1:解锁  2:上锁  
	uint8_t light;//用于灯闪烁的标志位
}_LOCK;

extern _LOCK _lock;

#endif



