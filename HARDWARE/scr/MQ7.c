#include "MQ7.h"
#include "delay.h"

//气体浓度值获取，adc
//当未达到阈值时，A0口基本为0V,所以该函数需在到达阈值时才能调用
//模块A0接PA2 adc2_ch2		对应PCB的引脚为AO_1
float Get_Smoke_value()
{	
	float voltage = 0;
	int ADC_value = 0;
	//HAL_ADC_Start(&hadc2);
	//HAL_ADC_PollForConversion(&hadc2,100);
	ADC_value = ADC_GetConversionValue(ADC2);
	voltage = threshold_value + (float)(ADC_value * 300/4096);			//单位为ppm，该模块测量范围为100~10000ppm
	return voltage;
}


//模块D0接PA3	对应PCB的引脚为DO_1
//气体浓度模块阈值检测
//返回值0：未到达阈值
//返回值1：已超过阈值
int Get_Smoke_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3);
	if(Pin_State)
	{
		DelayMs(1000);				//延时1秒，避免气体浓度不稳定
		Pin_State =  GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}
