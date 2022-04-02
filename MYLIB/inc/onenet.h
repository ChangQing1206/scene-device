#ifndef __ONENET_H
#define __ONENET_H

#include "stm32f10x.h"
#include "delay.h"
#include "stm32f10x.h"   //������Ҫ��ͷ�ļ�
#include "control.h"     //������Ҫ��ͷ�ļ�
#include "delay.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "timer2.h"      //������Ҫ��ͷ�ļ�
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�
#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "led.h"	     //������Ҫ��ͷ�ļ� LED                                                  
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "myiic.h"       //������Ҫ��ͷ�ļ� IIC
#include "adc.h"         //������Ҫ��ͷ�ļ� ADC
#include "SHT30.h"       //������Ҫ��ͷ�ļ� ������ʪ��
#include "MQ_7.h"        //������Ҫ��ͷ�ļ� ����
#include "light.h"       //������Ҫ��ͷ�ļ� ����
#include "beep.h"        //������Ҫ��ͷ�ļ� ������

void Onenet_Init(void);

#endif

