#include "light.h"
uint8_t buff2[200];

//��������ģ��ģ�������
//��ý�Ϊ��ȷ�ع��ն�
//����ֵ voltage
//PA4Ϊadc1 ch4		��ӦPCB������ΪAO_2
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



//��������ģ����ֵ���
//����ֵ0��δ������ֵ
//����ֵ1���ѳ�����ֵ
//ģ��D0��PA5		��ӦPCB������ΪDO_2
int Get_Light_state()
{
	uint8_t Pin_State = 0;
	Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5);
	if(Pin_State)
	{
		DelayMs(3000);				//��ʱ3�룬������ղ��ȶ�
		Pin_State = GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5);
		if(Pin_State)
			return 0;
		else
			return 1;
	}	
	else
		return 1;
}


