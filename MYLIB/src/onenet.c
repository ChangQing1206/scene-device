#include "onenet.h"
#include "led.h"


char *cmdLED_On = "LED0ON";      //LED打开
char *cmdLED_Off = "LED0OFF";    //LED关闭

// 定义上位机命令


//char *cmdDHT11_On = "DHTON";    //温湿度数据传输打开 ONENET下发数据命令
//char *cmdDHT11_Off = "DHTOFF";  //温湿度数据传输关闭


char *ledFlag = "1";            //LED0状态

void Onenet_Init(void)

	{		
     	// connectFlag=1同服务器建立了连接 
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if成立的话，说明发送缓冲区有数据了 ： 指针 + 缓冲区
			{                
				
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且connectPackFlag置位，表示连接报文成功
				//第3种：subcribePackFlag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					// 连接mqtt服务器
					if (MQTT_TxDataOutPtr[2] == 0x10) 
					{	
//						u1_printf("send:0x%x,SEND TO ONE \r\n", MQTT_TxDataOutPtr[2]);	
							u1_printf("发送数据:0x%x，单片机数据推送至服务器\r\n", MQTT_TxDataOutPtr[2]);	
						//u1_printf("\r\n");
					}
					else 
					{
						u1_printf("SEND_DATA:0x%x\r\n", MQTT_TxDataOutPtr[2]);//串口提示信息
					}
					MQTT_TxData(MQTT_TxDataOutPtr);							//发送数据 
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//指针下移
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//如果指针到缓冲区尾部了
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//指针归位到缓冲区开头
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if成立的话，说明接收缓冲区有数据了	
			{												
				u1_printf("RECV_DATA:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT\r\n");							//串口输出信息	
										connectPackFlag = 1;									//CONNECT报文成功，订阅报文可发
									break;														//跳出分支case 0x00                                              
						case 0x01 : u1_printf("CONNECT FAIL  ERROR VISION\r\n");    //串口输出信息
						//u1_printf("连接已拒绝，不支持的协议版本，准备重启\r\n");    //串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接
									break;														//跳出分支case 0x01   
						case 0x02 : u1_printf("CONNECT FAIL  ERROR ID\r\n\r\n");//串口输出信息
						//u1_printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");//串口输出信息
									connectFlag = 0;                                            //connectFlag置零，重启连接
									break; 														//跳出分支case 0x02 
						case 0x03 : u1_printf("CONNECT FAIL  ERROR SRRVICE\r\n");	    //串口输出信息
						//u1_printf("连接已拒绝，服务端不可用，准备重启\r\n");	    //串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接
									break;														//跳出分支case 0x03
						case 0x04 : u1_printf("CONNECT FAIL  ERROR USERNAME OR PASSWARD\r\n\r\n");	//串口输出信息
						//u1_printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");	//串口输出信息
									connectFlag = 0;											//connectFlag置零，重启连接						
									break;														//跳出分支case 0x04
						case 0x05 : u1_printf("CONNECT FAIL  ERROR WARRANTY\r\n");				//串口输出信息
						//u1_printf("连接已拒绝，未授权，准备重启\r\n");
									connectFlag = 0;											//connectFlag置零，重启连接						
									break;														//跳出分支case 0x05 		
						default   : u1_printf("CONNECT FAIL  RETRY CONNETC\r\n");		    //串口输出信息 
						//u1_printf("连接已拒绝，未知状态，准备重启\r\n");		    //串口输出信息 
									connectFlag = 0;											//connectFlag置零，重启连接					
									break;														//跳出分支case default 								
					}				
			  }			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									//u1_printf("订阅成功\r\n");			//串口输出信息
									u1_printf("SUB SUCCESS\r\n");			//串口输出信息
									subcribePackFlag = 1;				//subcribePackFlag置1，表示订阅报文成功，其他报文可发送
									pingFlag = 0;						//pingFlag清零
									TIM3_ENABLE_30S();					//启动30s的PING定时器	
									Send_Data();
									TIM2_ENABLE_10S();
									break;								//跳出分支                                             
						default: 
									//u1_printf("订阅失败，准备重启\r\n");//串口输出信息 
									u1_printf("SUB FAIL  RETRY\r\n");			//串口输出信息
									connectFlag = 0;					//connectFlag置零，重启连接
									break;								//跳出分支 								
						}					
			    }
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					//u1_printf("PING报文回复\r\n");                      //串口输出信息 
					u1_printf("PING RECV  \r\n");			//串口输出信息
					if(pingFlag == 1)
					{                   						        //如果pingFlag=1，表示第一次发送
						 pingFlag = 0;    				       			//要清除pingFlag标志
					}
					else if(pingFlag > 1)	
					{ 				 									//如果pingFlag>1，表示是多次发送了，而且是2s间隔的快速发送
						pingFlag = 0;     				      			//要清除pingFlag标志
						TIM3_ENABLE_30S(); 				      			//PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令  在下面处理
				else if((MQTT_RxDataOutPtr[2] == 0x30))
			    { 
					//u1_printf("服务器等级0推送\r\n"); 		   	//串口输出信息 
					u1_printf("SERVICE 0 PULISH \r\n");			//串口输出信息
					// 提取控制命令
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //处理等级0推送数据
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //指针下移
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //如果指针到缓冲区尾部了
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //指针归位到缓冲区开头              
				}		          
			}//处理接收缓冲区数据的else if分支结尾
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 处理命令缓冲区数据                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if成立的话，说明命令缓冲区有数据了	
			{                             		       
				//u1_printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);              //串口输出信息
				u1_printf("COMMAND:%s\r\n",&MQTT_CMDOutPtr[2]);              //串口输出信息
				if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//判断指令，如果是CMD1 
				{                                            
					ledFlag = "1";  								             
		//			LED0_On();									  		   //LED开启
					u1_printf("WINDOW OPEN\r\n");              //串口输出信息
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //判断指令，如果是CMD11
				{ 
					//ledFlag = "LED0OFF"; 
					ledFlag = "0";  					
	//				LED0_Off(); 								                //LED关闭
					u1_printf("WINDOW CLOSE\r\n");              //串口输出信息		
				}
							
				//不做处理，后面会发送状态
				else //u1_printf("未知指令\r\n");				//串口输出信息	
				u1_printf("unknow COMMAND\r\n");	
				MQTT_CMDOutPtr += CBUFF_UNIT;				//指针下移
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //如果指针到缓冲区尾部了
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//指针归位到缓冲区开头	
				//MQTT_PublishQs0()
				Send_Data();//发送控制数据
				//处理命令缓冲区数据的else if分支结尾					
			}	//connectFlag=1的if分支的结尾
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0同服务器断开了连接,我们要重启连接服务器            */
	/*--------------------------------------------------------------------*/
		else
		{ 
			//u1_printf("需要连接服务器\r\n");               //串口输出信息
			u1_printf("RETRY TO CONNETC SERVICE \r\n");               //串口输出信息
			TIM_Cmd(TIM4, DISABLE);                        //关闭TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //关闭TIM3  
			WiFi_RxCounter = 0;                            //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer() == 0)			   //如果WiFi连接云服务器函数返回0，表示正确，进入if
			{   			     
				//u1_printf("建立TCP连接成功\r\n");          //串口输出信息
				u1_printf("SUCCESSFUL CONNETC TCP\r\n");          //串口输出信息
				connectFlag = 1;                           //connectFlag置1，表示连接成功	
				WiFi_RxCounter = 0;                        //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //清空WiFi接收缓冲区 
				MQTT_Buff_Init();                          //初始化发送缓冲区                    
			}				
		}
	}
	