#ifndef __MQ_7_H
#define __MQ_7_H

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "adc.h"
#define threshold_value 0		//设定气体浓度阈值为 25

extern uint8_t buff1[200];
char *Get_Smoke_value(void);
int Get_Smoke_state(void);

#endif

