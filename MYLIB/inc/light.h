#ifndef __LIGHT_H
#define __LIGHT_H

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"

extern uint8_t buff2[200];
char *Get_Light_value(void);				//ģ��ģ�������
int Get_Light_state(void);							//ģ�����������

#endif
