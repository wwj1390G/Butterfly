#ifndef __QUERY_TAST_H
#define __QUERY_TAST_H
#include "stm32f10x.h"

void QueryTast(void);

typedef struct
{
	uint8_t lock_flag   : 2 ;//������־λ   0:�޲���  1:����  2:����  
	uint8_t light;//���ڵ���˸�ı�־λ
}_LOCK;

extern _LOCK _lock;

#endif



