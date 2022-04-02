 /*-----------------------------------------------------*/
/*                                                     */
/*            程序main函数，入口函数源文件              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h"   //	包含需要的头文件
#include "control.h"     //	包含需要的头文件
#include "delay.h"       //	包含需要的头文件
#include "usart1.h"      //	包含需要的头文件
#include "usart2.h"      //	包含需要的头文件

#include "timer2.h"      //	包含需要的头文件
#include "timer3.h"      //	包含需要的头文件
#include "timer4.h"      //	包含需要的头文件
#include "wifi.h"	     	//	包含需要的头文件
#include "led.h"	     	//	包含需要的头文件 LED                                                  
#include "mqtt.h"        //	包含需要的头文件
#include "myiic.h"       //	包含需要的头文件 IIC
#include "adc.h"         //	包含需要的头文件 ADC
#include "SHT30.h"       //	包含需要的头文件 空气温湿度			
#include "MQ_7.h"        //	包含需要的头文件 烟雾
#include "light.h"       //	包含需要的头文件 光照
#include "beep.h"        //	包含需要的头文件 蜂鸣器
#include "gps_config.h"         //	包含需要的头文件 GPS
//#include "dsb18b20.h"    // 包含需要的头文件 温度传感器
#include "onenet.h"

/*
   - stm32f10x_it.c：中断处理函数
   - usart1.c：与上位机通信
   - usart2.c：与ESP8266通信，串口2接收中断处理上位机发送来的数据
   - timer2.c：定时器2中断用来发送温湿度数据，10S
   - timer3.c：定时器3中断用来发送心跳包（ping，用于保持和服务器连接，长时间没给服务器发送数据会被踢下线），2s和30s两种模式
   - timer4.c：将串口2接收到的服务器数据依次存放在MQTT接收缓存数组中，50ms
   - control.c：控制LED灯，并且发送LED灯开关信号
*/



/*
任务：需要完成mqtt发送数据至服务器、服务器发送数据到本地
为了方便完成，发送的数据是写死的
1.先找到发送数据的地方  中断处理stm32f10x_it.c    写中断2里面写死数据    done
2.打开手机热点，连上WiFi模块，连上ch340模块，打开server.js 配置好端口 3010 运行server.js 连接stm32  done
3.测试成功  done
4.服务器下发数据，串口打印
5.gps数据上传完毕
6.测试ds18b20
*/



int main(void) 
{	
	Delay_Init(72);          //延时功能初始化              
	Usart1_Init(9600);     //串口1功能初始化，波特率9600
	Usart2_Init(115200);   //串口2功能初始化，波特率115200	
	TIM4_Init(500,7200);   //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
//	LED0_Init();			   //LED0初始化
//	LED1_Init();			   //LED1初始化
//	LED2_Init();			   //LED2初始化
//	IIC_Init();            //初始化IIC
//	SHT30_Init();          //初始化SHT30
//	ADC2_Init();    // 采集光照
//	ADC1_Init();
//	BEEP_Init();          //初始化beep
	// 改变方案  使用串口3与GPS通信
	GPS_Config();          // GPS 串口3 初始化
	
	WiFi_ResetIO_Init();   //初始化WiFi的复位IO
	IoT_Parameter_Init();  //初始化MQTT服务器的参数	
	while(1)
	{	

		Onenet_Init();

	}
	
}
