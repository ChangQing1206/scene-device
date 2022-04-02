#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"
#include "delay.h"

#define 	BEEP_ON 	GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define 	BEEP_OFF 	GPIO_ResetBits(GPIOB,GPIO_Pin_12)
void BEEP_Init(void);
void BEEP_Last(int delay_time);

#endif


