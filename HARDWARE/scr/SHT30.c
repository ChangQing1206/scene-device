#include "SHT30.h"
#include "myiic.h"
#include "delay.h"


float Temperature_sht;
float Humidity_sht;

uint8_t humiture_buff1[100];
u8 Refresh_SHT30_Data = 0;
u8 send_data_fleg;


void SHT30_Init(void)
{
	
	//DEBUG_USARTx = USART1;
	//printf("SHT30 has initialized!\r\n");
}

void SHT30_read_result(u8 addr)
{
	u16 tem, hum;
	u16 buff[6];
	float Temperature_Value = 0;
	float Humidity_Value = 0;	
	
	IIC_Start();
	IIC_Send_Byte(addr<<1 | write);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x2C);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x06);
	IIC_Wait_Ack();
	IIC_Stop();
	DelayMs(50);
	IIC_Start();
	IIC_Send_Byte(addr<<1 | read);
	if(IIC_Wait_Ack() == 0)
	{
		buff[0] = IIC_Read_Byte(1);
		buff[1] = IIC_Read_Byte(1);
		buff[2] = IIC_Read_Byte(1);
		buff[3] = IIC_Read_Byte(1);
		buff[4] = IIC_Read_Byte(1);
		buff[5] = IIC_Read_Byte(0);
		IIC_Stop();
	}
	tem = ((buff[0]<<8) | buff[1]);//温度拼接
	hum = ((buff[3]<<8) | buff[4]);//湿度拼接
	/*转换实际温度*/
	Temperature_Value = (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity_Value = (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	
	if((Temperature_Value>=-20)&&(Temperature_Value<=125)&&(Humidity_Value>=0)&&(Humidity_Value<=100))//过滤错误
	{
		Temperature_sht = Temperature_Value;
		Humidity_sht = Humidity_Value;
//		sprintf(humiture_buff1,"%6.2f*C %6.2f%%",Temperature,Humidity);//111.01*C 100.01%（保留2位小数）
	}
//	printf("温湿度：%s\r\n", humiture_buff1);
	hum = 0;
	tem = 0;
}
