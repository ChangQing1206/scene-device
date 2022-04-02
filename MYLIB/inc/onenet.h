#ifndef __ONENET_H
#define __ONENET_H

#include "stm32f10x.h"
#include "delay.h"
#include "stm32f10x.h"   //包含需要的头文件
#include "control.h"     //包含需要的头文件
#include "delay.h"       //包含需要的头文件
#include "usart1.h"      //包含需要的头文件
#include "usart2.h"      //包含需要的头文件
#include "timer2.h"      //包含需要的头文件
#include "timer3.h"      //包含需要的头文件
#include "timer4.h"      //包含需要的头文件
#include "wifi.h"	     //包含需要的头文件
#include "led.h"	     //包含需要的头文件 LED                                                  
#include "mqtt.h"        //包含需要的头文件
#include "myiic.h"       //包含需要的头文件 IIC
#include "adc.h"         //包含需要的头文件 ADC
#include "SHT30.h"       //包含需要的头文件 空气温湿度
#include "MQ_7.h"        //包含需要的头文件 烟雾
#include "light.h"       //包含需要的头文件 光照
#include "beep.h"        //包含需要的头文件 蜂鸣器

void Onenet_Init(void);

#endif

