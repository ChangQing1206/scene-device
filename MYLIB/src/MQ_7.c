#include "MQ_7.h"
#include "beep.h"
uint8_t buff1[200];

//����Ũ��ֵ��ȡ��adc
//��δ�ﵽ��ֵʱ��A0�ڻ���Ϊ0V,���Ըú������ڵ�����ֵʱ���ܵ���
//ģ��A0��PA6 adc1_ch6		��ӦPCB������ΪAO_1
char *Get_Smoke_value()
{	
	
	float voltage = 0;
	int ADC2_value = 0;
//	char voltage_mq7[4]; 
	ADC2_Init();
	ADC2_value = ADC_GetConversionValue(ADC2);
	voltage = (float)(ADC2_value * 3.3/4096);			//��λΪppm����ģ�������ΧΪ100~10000ppm
	sprintf(buff1, "%.2f", voltage);
	return buff1;
	if(voltage>2.5)
	{
		LED0_On();
		BEEP_Last(1000);
	}
}


//ģ��D0��PA7	��ӦPCB������ΪDO_1
//����Ũ��ģ����ֵ���
//����ֵ0��δ������ֵ
//����ֵ1���ѳ�����ֵ
int Get_Smoke_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7);
	if(Pin_State)
	{
		//DelayMs(1000);				//��ʱ1�룬��������Ũ�Ȳ��ȶ�
		Pin_State =  GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}
