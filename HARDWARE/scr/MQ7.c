#include "MQ7.h"
#include "delay.h"

//����Ũ��ֵ��ȡ��adc
//��δ�ﵽ��ֵʱ��A0�ڻ���Ϊ0V,���Ըú������ڵ�����ֵʱ���ܵ���
//ģ��A0��PA2 adc2_ch2		��ӦPCB������ΪAO_1
float Get_Smoke_value()
{	
	float voltage = 0;
	int ADC_value = 0;
	//HAL_ADC_Start(&hadc2);
	//HAL_ADC_PollForConversion(&hadc2,100);
	ADC_value = ADC_GetConversionValue(ADC2);
	voltage = threshold_value + (float)(ADC_value * 300/4096);			//��λΪppm����ģ�������ΧΪ100~10000ppm
	return voltage;
}


//ģ��D0��PA3	��ӦPCB������ΪDO_1
//����Ũ��ģ����ֵ���
//����ֵ0��δ������ֵ
//����ֵ1���ѳ�����ֵ
int Get_Smoke_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3);
	if(Pin_State)
	{
		DelayMs(1000);				//��ʱ1�룬��������Ũ�Ȳ��ȶ�
		Pin_State =  GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}
