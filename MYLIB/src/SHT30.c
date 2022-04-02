#include "SHT30.h"
#include "myiic.h"
#include "delay.h"
#include "sys.h"

float humiture[4];
u8 Refresh_SHT30_Data = 0;
u8 humiture_buff1[20];
u8 humiture_buff2[20];

u8 send_data_fleg;

u8 Temperature_L = 15;
u8 Humidity_L = 50;
u8 Temperature_H = 30;
u8 Humidity_H = 80;

float Temperature_sht;
float Humidity_sht;

void SHT30_Init(void)
{
	printf("SHT30 has initialized!\r\n");
}

void SHT30_read_result(u8 addr)
{
	u16 tem, hum;
	u16 buf[6];

	
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
		buf[0] = IIC_Read_Byte(1);
		buf[1] = IIC_Read_Byte(1);
		buf[2] = IIC_Read_Byte(1);
		buf[3] = IIC_Read_Byte(1);
		buf[4] = IIC_Read_Byte(1);
		buf[5] = IIC_Read_Byte(0);
		IIC_Stop();
	}
	tem = ((buf[0]<<8) | buf[1]);//温度拼接
	hum = ((buf[3]<<8) | buf[4]);//湿度拼接
	/*转换实际温度*/
	Temperature_sht = (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity_sht = (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	if((Temperature_sht>=-20)&&(Temperature_sht<=125)&&(Humidity_sht>=0)&&(Humidity_sht<=100))//过滤错误
	{
//		humiture[0] = Temperature;
//		humiture[2] = Humidity;
//		sprintf(humiture_buff1,"%6.2f*C %6.2f%%",Temperature_sht,Humidity_sht);//111.01*C 100.01%（保留2位小数）
	}
//	printf("Humidity is %f\r\n", Humidity);
//	printf("温湿度：%s\r\n", humiture_buff1);
	hum = 0;
	tem = 0;
}
