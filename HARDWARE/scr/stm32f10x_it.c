/*-------------------------------------------------*/
/*                                                 */
/*            	   中断服务函数          	   	   */
/*                                                 */
/*-------------------------------------------------*/
//#include "stdio.h"
//#include "string.h"
#include "stm32f10x.h"     //包含需要的头文件
#include "stm32f10x_it.h"  //包含需要的头文件
#include "usart1.h"        //包含需要的头文件
#include "usart2.h"        //包含需要的头文件
#include "timer3.h"        //包含需要的头文件
#include "mqtt.h"          //包含需要的头文件
#include "SHT30.h"         //包含需要的头文件
#include "MQ_7.h"		   //包含需要的头文件 
#include "light.h"		   //包含需要的头文件 
#include "nema.h"         // GPS数据解码
#include "gps_config.h"


/*-------------------------------------------------*/
/*函数名：串口2接收中断函数（最高优先级，处理接收数据）*/
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{  
		if(connectFlag == 0) 							     //如果connectFlag等于0，当前还没有连接服务器，处于指令配置状态
		{                                     
			if(USART2->DR)
			{                                     			 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;//保存到缓冲区	
				Usart2_RxCounter++; 						 //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}					
		}
		else
		{		                                           	 //反之connectFlag等于1，连接上服务器了	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart2_RxCounter == 0)
			{    									    											 //如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4, ENABLE); 
			}
			else																						 //else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
			{                        									    
				TIM_SetCounter(TIM4, 0);  
			}	
			Usart2_RxCounter++;         				     //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
} 

/*中断优先级
 *1.EXTI
 *2.TIM3
 *3.TIM2
 *4.USART1
 *5.USART2
 */

/**
  * @brief  This function handles USART3 interrupt request
  * @param
  */ 
// 串口3 中断处理函数
void USART3_IRQHandler(void)
{ 
	// temp_gpsdata[i]
	char res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		res = USART_ReceiveData(USART3);
		// 寻找$GPGLL
		if(res == '$') {
			i = 0;
		}
		temp_gpsdata[i++] = res;
		if(temp_gpsdata[0] == '$' && temp_gpsdata[2] == 'N' && temp_gpsdata[4] == 'L' && temp_gpsdata[5] == 'L')  // $GNGLL
		{
			if(res == '\n') {
				// 找到了这帧数据的尾部
				USART_RX_OK_FLAG = 1; // 接收完一帧数据后置位1
			}
		}			
		
	}
}
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数。处理MQTT数据          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	// 定时器计数达到指定值（一定时间后）溢出 去处理数据
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);  //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                  	//记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;					 	//记录数据长度低字节
		MQTT_RxDataInPtr += RBUFF_UNIT;                                	//指针下移
		if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//如果指针到缓冲区尾部了
				MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//指针归位到缓冲区开头
		Usart2_RxCounter = 0;                                        	//串口2接收数据量变量清零
		TIM_SetCounter(TIM3, 0);                                     	//清零定时器3计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);                        				 	//关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			 	//清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			 	//清除TIM4溢出中断标志 	
	}
}
/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
		{  
		switch(pingFlag) 					//判断pingFlag的状态
		{                               
			case 0:							//如果pingFlag等于0，表示正常状态，发送Ping报文  
					MQTT_PingREQ(); 		//添加Ping报文到发送缓冲区  
					break;
			case 1:							//如果pingFlag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
					TIM3_ENABLE_2S(); 	    //我们将定时器6设置为2s定时,快速发送Ping报文
					MQTT_PingREQ();			//添加Ping报文到发送缓冲区  
					break;
			case 2:							//如果pingFlag等于2，说明还没有收到服务器回复
			case 3:				            //如果pingFlag等于3，说明还没有收到服务器回复
			case 4:				            //如果pingFlag等于4，说明还没有收到服务器回复	
					MQTT_PingREQ();  		//添加Ping报文到发送缓冲区 
					break;
			case 5:							//如果pingFlag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
					connectFlag = 0;        //连接状态置0，表示断开，没连上服务器
					TIM_Cmd(TIM3, DISABLE); //关TIM3 				
					break;			
		}
		pingFlag++;           		   		//pingFlag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIM3溢出中断标志 	
	}
}

/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*        设备上传数据                                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{

	char head2[3];				//定义一个报头临时缓冲区
	char head3[3];				//定义一个报头临时缓冲区
	char head4[3];				//定义一个报头临时缓冲区
	
	char temp2[50];			//定义一个临时缓冲区,不包括报头
	char temp3[50];			    //定义一个临时缓冲区,不包括报头
	char humiture_buff1[100];	//定义一个临时缓冲区,不包括报

	char tempAll2[100];			//定义一个临时缓冲区，包括所有数据
	char tempAll3[100];			//定义一个临时缓冲区，包括所有数据
	char tempAll4[150];			//定义一个临时缓冲区，包括所有数据
	

	int	dataLen2 = 0;			//报文长度
	int	dataLen3 = 0;			//报文长度
	int	dataLen4 = 0;			//报文长度
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{ 
		
		// 读取数据
//		SHT30_read_result(0x44);  				//读取温湿度值
//		Get_Light_value();						//读取光照值
//		Get_Smoke_value();						//读取空气质量
		// 写死的数据
		sprintf(buff1, "%.2f", 22.2);
		sprintf(buff2, "%.2f", 88.8);
//		Temperature_sht = 28.3;
//		Humidity_sht = 50.4;
//		nema_decode_test();   
		GET_GPS();  // 取得经纬度
		
		memset(temp2,    0, 50);				//清空缓冲区1_mq7
		memset(temp3,    0, 50);				//清空缓冲区1_light
		memset(humiture_buff1,  0,100);			//清空缓冲区1_sht30

		memset(tempAll2,  0, 50);				//清空缓冲区2
		memset(tempAll3,  0, 100);				//清空缓冲区3
		memset(tempAll4,  0, 150);				//清空缓冲区4

		memset(head2,    0, 3);					//清空MQTT头
		memset(head3,    0, 3);					//清空MQTT头
		memset(head4,    0, 3);					//清空MQTT头

//		sprintf(temp1,"{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\"}",temperature/10, temperature%10, humidity/10, humidity%10);//构建报文

		// buff1 是mq-7里面的缓冲区  buff2 是光照数据的缓冲区  最后一个是温湿度数据
		sprintf(temp2,"{\"voltage_mq7\":\"%s\"}", buff1);//构建报文
		sprintf(temp3,"{\"voltage_light\":\"%s\"}", buff2);//构建报文
//		sprintf(humiture_buff1,"{\"Temperature_sht\":\"%6.2f\",\"Humidity_sht\":\"%6.2f\"}",Temperature_sht,Humidity_sht);//111.01*C 100.01%（保留2位小数）
		sprintf(humiture_buff1,"{\"longtitude\":\"%.6lf\",\"latitude\":\"%.6lf\"}",longtitude,latitude); // double类型 lf .6 即是6位小数
  
/*-------------------------------------------------*/
		head2[0] = 0x03; 						//固定报头
		head2[1] = 0x00; 						//固定报头
		head2[2] = strlen(temp2);  				//剩余长度
		head3[0] = 0x03; 						//固定报头
		head3[1] = 0x00; 						//固定报头
		head3[2] = strlen(temp3);  				//剩余长度	
		head4[0] = 0x03; 						//固定报头
		head4[1] = 0x00; 						//固定报头
		head4[2] = strlen(humiture_buff1);  	//剩余长度	
			
		// 填充烟雾数据
		sprintf(tempAll2, "%c%c%c%s", head2[0], head2[1], head2[2], temp2);   // tempAll2 是烟雾数据 + 固定报头
		//串口显示相关数据
		u1_printf("\r\n");   // 打印回车
		u1_printf("%s\r\n", tempAll2 + 3);   // 打印要发送的数据
	  // 填充光照数据
		sprintf(tempAll3, "%c%c%c%s", head3[0], head3[1], head3[2], temp3);
		//串口显示相关数据
		u1_printf("\r\n");   // 打印回车
		u1_printf("%s\r\n", tempAll3 + 3);   // 打印要发送的数据
		// 填充温湿度数据
		sprintf(tempAll4, "%c%c%c%s", head4[0], head4[1], head4[2], humiture_buff1);
		//串口显示相关数据
		u1_printf("\r\n");    // 打印回车
		u1_printf("%s\r\n", tempAll4 + 3);   // 打印要发送的数据

//		dataLen1 = strlen(temp1)+ 3;
    // 计算数据长度
		dataLen2 = strlen(temp2)+ 3;
		// mqtt 发送数据给服务器
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll2, dataLen2);//添加数据，发布给服务器
		dataLen3 = strlen(temp3)+ 3;
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll3, dataLen3);//添加数据，发布给服务器
		dataLen4 = strlen(humiture_buff1)+ 3;
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll4, dataLen4);//添加数据，发布给服务器
    // 清除一些中断标志位

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   	
	}
}

/*-------------------------------------------------*/
/*函数名：不可屏蔽中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：硬件出错后进入的中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：内存管理中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：预取指失败，存储器访问失败中断处理函数   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：未定义的指令或非法状态处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*函数名：软中断，SWI 指令调用的处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：调试监控器处理函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*函数名：可挂起的系统服务处理函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：SysTic系统嘀嗒定时器处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}
/*-------------------------------------------------*/
/*函数名：GPS DMA中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DMA1_Channel3_IRQHandler(void)
{
  
  GPS_ProcessDMAIRQ();

}
