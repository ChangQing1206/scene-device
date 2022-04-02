#include "MQ_7.h"
#include "beep.h"
uint8_t buff1[200];

//气体浓度值获取，adc
//当未达到阈值时，A0口基本为0V,所以该函数需在到达阈值时才能调用
//模块A0接PA6 adc1_ch6		对应PCB的引脚为AO_1
char *Get_Smoke_value()
{	
	
	float voltage = 0;
	int ADC2_value = 0;
//	char voltage_mq7[4]; 
	ADC2_Init();
	ADC2_value = ADC_GetConversionValue(ADC2);
	voltage = (float)(ADC2_value * 3.3/4096);			//单位为ppm，该模块测量范围为100~10000ppm
	sprintf(buff1, "%.2f", voltage);
	return buff1;
	if(voltage>2.5)
	{
		LED0_On();
		BEEP_Last(1000);
	}
}


//模块D0接PA7	对应PCB的引脚为DO_1
//气体浓度模块阈值检测
//返回值0：未到达阈值
//返回值1：已超过阈值
int Get_Smoke_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7);
	if(Pin_State)
	{
		//DelayMs(1000);				//延时1秒，避免气体浓度不稳定
		Pin_State =  GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}
