#include "onenet.h"
#include "led.h"


char *cmdLED_On = "LED0ON";      //LED��
char *cmdLED_Off = "LED0OFF";    //LED�ر�

// ������λ������


//char *cmdDHT11_On = "DHTON";    //��ʪ�����ݴ���� ONENET�·���������
//char *cmdDHT11_Off = "DHTOFF";  //��ʪ�����ݴ���ر�


char *ledFlag = "1";            //LED0״̬

void Onenet_Init(void)

	{		
     	// connectFlag=1ͬ���������������� 
		if(connectFlag == 1) 
		{     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ������������� �� ָ�� + ������
			{                
				
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���connectPackFlag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�subcribePackFlag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					// ����mqtt������
					if (MQTT_TxDataOutPtr[2] == 0x10) 
					{	
//						u1_printf("send:0x%x,SEND TO ONE \r\n", MQTT_TxDataOutPtr[2]);	
							u1_printf("��������:0x%x����Ƭ������������������\r\n", MQTT_TxDataOutPtr[2]);	
						//u1_printf("\r\n");
					}
					else 
					{
						u1_printf("SEND_DATA:0x%x\r\n", MQTT_TxDataOutPtr[2]);//������ʾ��Ϣ
					}
					MQTT_TxData(MQTT_TxDataOutPtr);							//�������� 
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//ָ������
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//���ָ�뵽������β����
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//ָ���λ����������ͷ
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
			{												
				u1_printf("RECV_DATA:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2] == 0x20)
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT\r\n");							//���������Ϣ	
										connectPackFlag = 1;									//CONNECT���ĳɹ������ı��Ŀɷ�
									break;														//������֧case 0x00                                              
						case 0x01 : u1_printf("CONNECT FAIL  ERROR VISION\r\n");    //���������Ϣ
						//u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x01   
						case 0x02 : u1_printf("CONNECT FAIL  ERROR ID\r\n\r\n");//���������Ϣ
						//u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");//���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break; 														//������֧case 0x02 
						case 0x03 : u1_printf("CONNECT FAIL  ERROR SRRVICE\r\n");	    //���������Ϣ
						//u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");	    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x03
						case 0x04 : u1_printf("CONNECT FAIL  ERROR USERNAME OR PASSWARD\r\n\r\n");	//���������Ϣ
						//u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");	//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x04
						case 0x05 : u1_printf("CONNECT FAIL  ERROR WARRANTY\r\n");				//���������Ϣ
						//u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x05 		
						default   : u1_printf("CONNECT FAIL  RETRY CONNETC\r\n");		    //���������Ϣ 
						//u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");		    //���������Ϣ 
									connectFlag = 0;											//connectFlag���㣬��������					
									break;														//������֧case default 								
					}				
			  }			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2] == 0x90)
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									//u1_printf("���ĳɹ�\r\n");			//���������Ϣ
									u1_printf("SUB SUCCESS\r\n");			//���������Ϣ
									subcribePackFlag = 1;				//subcribePackFlag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									pingFlag = 0;						//pingFlag����
									TIM3_ENABLE_30S();					//����30s��PING��ʱ��	
									Send_Data();
									TIM2_ENABLE_10S();
									break;								//������֧                                             
						default: 
									//u1_printf("����ʧ�ܣ�׼������\r\n");//���������Ϣ 
									u1_printf("SUB FAIL  RETRY\r\n");			//���������Ϣ
									connectFlag = 0;					//connectFlag���㣬��������
									break;								//������֧ 								
						}					
			    }
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2] == 0xD0)
				{ 
					//u1_printf("PING���Ļظ�\r\n");                      //���������Ϣ 
					u1_printf("PING RECV  \r\n");			//���������Ϣ
					if(pingFlag == 1)
					{                   						        //���pingFlag=1����ʾ��һ�η���
						 pingFlag = 0;    				       			//Ҫ���pingFlag��־
					}
					else if(pingFlag > 1)	
					{ 				 									//���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						pingFlag = 0;     				      			//Ҫ���pingFlag��־
						TIM3_ENABLE_30S(); 				      			//PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������  �����洦��
				else if((MQTT_RxDataOutPtr[2] == 0x30))
			    { 
					//u1_printf("�������ȼ�0����\r\n"); 		   	//���������Ϣ 
					u1_printf("SERVICE 0 PULISH \r\n");			//���������Ϣ
					// ��ȡ��������
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
				}		          
			}//������ջ��������ݵ�else if��֧��β
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 ���������������                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if�����Ļ���˵�����������������	
			{                             		       
				//u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				u1_printf("COMMAND:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//�ж�ָ������CMD1 
				{                                            
					ledFlag = "1";  								             
		//			LED0_On();									  		   //LED����
					u1_printf("WINDOW OPEN\r\n");              //���������Ϣ
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //�ж�ָ������CMD11
				{ 
					//ledFlag = "LED0OFF"; 
					ledFlag = "0";  					
	//				LED0_Off(); 								                //LED�ر�
					u1_printf("WINDOW CLOSE\r\n");              //���������Ϣ		
				}
							
				//������������ᷢ��״̬
				else //u1_printf("δָ֪��\r\n");				//���������Ϣ	
				u1_printf("unknow COMMAND\r\n");	
				MQTT_CMDOutPtr += CBUFF_UNIT;				//ָ������
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //���ָ�뵽������β����
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//ָ���λ����������ͷ	
				//MQTT_PublishQs0()
				Send_Data();//���Ϳ�������
				//��������������ݵ�else if��֧��β					
			}	//connectFlag=1��if��֧�Ľ�β
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����            */
	/*--------------------------------------------------------------------*/
		else
		{ 
			//u1_printf("��Ҫ���ӷ�����\r\n");               //���������Ϣ
			u1_printf("RETRY TO CONNETC SERVICE \r\n");               //���������Ϣ
			TIM_Cmd(TIM4, DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //�ر�TIM3  
			WiFi_RxCounter = 0;                            //WiFi������������������                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer() == 0)			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			     
				//u1_printf("����TCP���ӳɹ�\r\n");          //���������Ϣ
				u1_printf("SUCCESSFUL CONNETC TCP\r\n");          //���������Ϣ
				connectFlag = 1;                           //connectFlag��1����ʾ���ӳɹ�	
				WiFi_RxCounter = 0;                        //WiFi������������������                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //���WiFi���ջ����� 
				MQTT_Buff_Init();                          //��ʼ�����ͻ�����                    
			}				
		}
	}
	