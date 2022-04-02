#include "light.h"
uint8_t buff2[200];

//光敏电阻模块模拟量检测
//获得较为精确地光照度
//返回值 voltage
//PA4为adc1 ch4		对应PCB的引脚为AO_2
char *Get_Light_value()
{
	float voltage= 0;
	int ADC1_value = 0;
//	char voltage_light[4];
	ADC1_Init();
	ADC1_value = ADC_GetConversionValue(ADC1);
	voltage = (float)(ADC1_value * 3.3/4096);
	sprintf(buff2, "%.2f", voltage);
	
	return buff2;
}



//光敏电阻模块阈值检测
//返回值0：未到达阈值
//返回值1：已超过阈值
//模块D0接PA5		对应PCB的引脚为DO_2
int Get_Light_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5);
	if(Pin_State)
	{
		DelayMs(3000);				//延时3秒，避免光照不稳定
		Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}


