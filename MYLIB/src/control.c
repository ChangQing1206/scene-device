/*-------------------------------------------------*/
/*                                                 */
/*                 控制端口           		   	   */
/*                                                 */
/*-------------------------------------------------*/

//LED控制数据发送给服务器

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "mqtt.h"

extern char *ledFlag;               //补光灯状态                   

/*-------------------------------------------------*/
/*函数名：数据长度 length 函数                      */
/*参  数：无                                       */
/*返回值：长度                 				       */
/*-------------------------------------------------*/
int length(int a)
{
	int i = 1;
	while(a/10 != 0)
	{
		i++;
		a = a/10;
	}
	return i;
}

/*-------------------------------------------------*/
/*函数名：发送控制设备数据                          */
/*参  数：无                                       */
/*返回值：							               */
/*-------------------------------------------------*/
void Send_Data(void)
{		
/*-------------------------------------------------*/

	char  head2[3];
	char  temp2[50];          	//定义一个临时缓冲区1,不包括报头
	char  tempAll2[100];       	//定义一个临时缓冲区2，包括所有数据
	char  head3[3];
	char  temp3[50];          	//定义一个临时缓冲区1,不包括报头
	char  tempAll3[100];       	//定义一个临时缓冲区2，包括所有数据
	char  head4[3];
	char  humiture_buff1[100];    //定义一个临时缓冲区1,不包括报头
	char  tempAll4[150];       	//定义一个临时缓冲区2，包括所有数据

//	//u16  ms;

	int   dataLen2;     	  	    //报文长度	
	int   dataLen3;     	  	    //报文长度	
	int   dataLen4;     	  	    //报文长度
	

	memset(temp2,       0, 50);  //清空缓冲区1
	memset(tempAll2,    0, 100); //清空缓冲区2
	memset(head2,       0, 3);   //清空MQTT头 
	memset(temp3,       0, 50);  //清空缓冲区1
	memset(tempAll3,    0, 100); //清空缓冲区2
	memset(head3,       0, 3);   //清空MQTT头 
	memset(humiture_buff1,0, 100);  //清空缓冲区1
	memset(tempAll4,    0, 150); //清空缓冲区2
	memset(head4,       0, 3);   //清空MQTT头    	
	

	sprintf(temp2, "{\"ledFlag_mq7\":\"%s\"}", ledFlag);//构建报文
	sprintf(temp3, "{\"ledFlag_light\":\"%s\"}", ledFlag);//构建报文
	sprintf(humiture_buff1, "{\"ledFlag\":\"%s\"}", ledFlag);//构建报文
	
	//sprintf(temp, "hello");//构建报文

	head2[0] = 0x03; //固定报头
	head2[1] = 0x00; //固定报头
	head2[2] = strlen(temp2); //剩余长度	
	sprintf(tempAll2, "%c%c%c%s", head2[0], head2[1], head2[2], temp2);
	head3[0] = 0x03; //固定报头
	head3[1] = 0x00; //固定报头
	head3[2] = strlen(temp3); //剩余长度	
	sprintf(tempAll3, "%c%c%c%s", head3[0], head3[1], head3[2], temp3);
	head4[0] = 0x03; //固定报头
	head4[1] = 0x00; //固定报头
	head4[2] = strlen(humiture_buff1); //剩余长度	
	sprintf(tempAll4, "%c%c%c%s", head4[0], head4[1], head4[2], humiture_buff1);
	
	//ms = 10000;


	dataLen2 = strlen(temp2) + 3;
	u1_printf("all: %s\r\n", tempAll2+3);
	dataLen3 = strlen(temp3) + 3;
	u1_printf("all: %s\r\n", tempAll3+3);
	dataLen4 = strlen(humiture_buff1) + 3;
	u1_printf("all: %s\r\n", tempAll4+3);
	
	//u1_printf("总长度：%d\r\n",Init_len);
	//DelayMs(ms);

	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll2, dataLen2); //添加数据，发布给服务
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll3, dataLen3); //添加数据，发布给服务
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll4, dataLen4); //添加数据，发布给服
	//这部分是重点，怎么处理数据发送到topic，目前直接调用上面函数可以发送固定报头到QT但是数据内容并没有发送过去
	
	
	
	
	
	
	
	
}	
