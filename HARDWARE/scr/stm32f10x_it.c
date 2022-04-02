/*-------------------------------------------------*/
/*                                                 */
/*            	   �жϷ�����          	   	   */
/*                                                 */
/*-------------------------------------------------*/
//#include "stdio.h"
//#include "string.h"
#include "stm32f10x.h"     //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h"  //������Ҫ��ͷ�ļ�
#include "usart1.h"        //������Ҫ��ͷ�ļ�
#include "usart2.h"        //������Ҫ��ͷ�ļ�
#include "timer3.h"        //������Ҫ��ͷ�ļ�
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "SHT30.h"         //������Ҫ��ͷ�ļ�
#include "MQ_7.h"		   //������Ҫ��ͷ�ļ� 
#include "light.h"		   //������Ҫ��ͷ�ļ� 
#include "nema.h"         // GPS���ݽ���
#include "gps_config.h"


/*-------------------------------------------------*/
/*������������2�����жϺ�����������ȼ�������������ݣ�*/
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
	{  
		if(connectFlag == 0) 							     //���connectFlag����0����ǰ��û�����ӷ�����������ָ������״̬
		{                                     
			if(USART2->DR)
			{                                     			 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;//���浽������	
				Usart2_RxCounter++; 						 //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}					
		}
		else
		{		                                           	 //��֮connectFlag����1�������Ϸ�������	
			Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;    //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart2_RxCounter == 0)
			{    									    											 //���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4, ENABLE); 
			}
			else																						 //else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
			{                        									    
				TIM_SetCounter(TIM4, 0);  
			}	
			Usart2_RxCounter++;         				     //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
	}
} 

/*�ж����ȼ�
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
// ����3 �жϴ�����
void USART3_IRQHandler(void)
{ 
	// temp_gpsdata[i]
	char res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		res = USART_ReceiveData(USART3);
		// Ѱ��$GPGLL
		if(res == '$') {
			i = 0;
		}
		temp_gpsdata[i++] = res;
		if(temp_gpsdata[0] == '$' && temp_gpsdata[2] == 'N' && temp_gpsdata[4] == 'L' && temp_gpsdata[5] == 'L')  // $GNGLL
		{
			if(res == '\n') {
				// �ҵ�����֡���ݵ�β��
				USART_RX_OK_FLAG = 1; // ������һ֡���ݺ���λ1
			}
		}			
		
	}
}
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����������MQTT����          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	// ��ʱ�������ﵽָ��ֵ��һ��ʱ������ ȥ��������
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
	{                	
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);  //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter/256;                  	//��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter%256;					 	//��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr += RBUFF_UNIT;                                	//ָ������
		if(MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                     	//���ָ�뵽������β����
				MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                    	//ָ���λ����������ͷ
		Usart2_RxCounter = 0;                                        	//����2������������������
		TIM_SetCounter(TIM3, 0);                                     	//���㶨ʱ��3�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);                        				 	//�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			 	//���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			 	//���TIM4����жϱ�־ 	
	}
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
		{  
		switch(pingFlag) 					//�ж�pingFlag��״̬
		{                               
			case 0:							//���pingFlag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 		//���Ping���ĵ����ͻ�����  
					break;
			case 1:							//���pingFlag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
					TIM3_ENABLE_2S(); 	    //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
					MQTT_PingREQ();			//���Ping���ĵ����ͻ�����  
					break;
			case 2:							//���pingFlag����2��˵����û���յ��������ظ�
			case 3:				            //���pingFlag����3��˵����û���յ��������ظ�
			case 4:				            //���pingFlag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  		//���Ping���ĵ����ͻ����� 
					break;
			case 5:							//���pingFlag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					connectFlag = 0;        //����״̬��0����ʾ�Ͽ���û���Ϸ�����
					TIM_Cmd(TIM3, DISABLE); //��TIM3 				
					break;			
		}
		pingFlag++;           		   		//pingFlag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIM3����жϱ�־ 	
	}
}

/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*        �豸�ϴ�����                                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{

	char head2[3];				//����һ����ͷ��ʱ������
	char head3[3];				//����һ����ͷ��ʱ������
	char head4[3];				//����һ����ͷ��ʱ������
	
	char temp2[50];			//����һ����ʱ������,��������ͷ
	char temp3[50];			    //����һ����ʱ������,��������ͷ
	char humiture_buff1[100];	//����һ����ʱ������,��������

	char tempAll2[100];			//����һ����ʱ��������������������
	char tempAll3[100];			//����һ����ʱ��������������������
	char tempAll4[150];			//����һ����ʱ��������������������
	

	int	dataLen2 = 0;			//���ĳ���
	int	dataLen3 = 0;			//���ĳ���
	int	dataLen4 = 0;			//���ĳ���
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	
	{ 
		
		// ��ȡ����
//		SHT30_read_result(0x44);  				//��ȡ��ʪ��ֵ
//		Get_Light_value();						//��ȡ����ֵ
//		Get_Smoke_value();						//��ȡ��������
		// д��������
		sprintf(buff1, "%.2f", 22.2);
		sprintf(buff2, "%.2f", 88.8);
//		Temperature_sht = 28.3;
//		Humidity_sht = 50.4;
//		nema_decode_test();   
		GET_GPS();  // ȡ�þ�γ��
		
		memset(temp2,    0, 50);				//��ջ�����1_mq7
		memset(temp3,    0, 50);				//��ջ�����1_light
		memset(humiture_buff1,  0,100);			//��ջ�����1_sht30

		memset(tempAll2,  0, 50);				//��ջ�����2
		memset(tempAll3,  0, 100);				//��ջ�����3
		memset(tempAll4,  0, 150);				//��ջ�����4

		memset(head2,    0, 3);					//���MQTTͷ
		memset(head3,    0, 3);					//���MQTTͷ
		memset(head4,    0, 3);					//���MQTTͷ

//		sprintf(temp1,"{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\"}",temperature/10, temperature%10, humidity/10, humidity%10);//��������

		// buff1 ��mq-7����Ļ�����  buff2 �ǹ������ݵĻ�����  ���һ������ʪ������
		sprintf(temp2,"{\"voltage_mq7\":\"%s\"}", buff1);//��������
		sprintf(temp3,"{\"voltage_light\":\"%s\"}", buff2);//��������
//		sprintf(humiture_buff1,"{\"Temperature_sht\":\"%6.2f\",\"Humidity_sht\":\"%6.2f\"}",Temperature_sht,Humidity_sht);//111.01*C 100.01%������2λС����
		sprintf(humiture_buff1,"{\"longtitude\":\"%.6lf\",\"latitude\":\"%.6lf\"}",longtitude,latitude); // double���� lf .6 ����6λС��
  
/*-------------------------------------------------*/
		head2[0] = 0x03; 						//�̶���ͷ
		head2[1] = 0x00; 						//�̶���ͷ
		head2[2] = strlen(temp2);  				//ʣ�೤��
		head3[0] = 0x03; 						//�̶���ͷ
		head3[1] = 0x00; 						//�̶���ͷ
		head3[2] = strlen(temp3);  				//ʣ�೤��	
		head4[0] = 0x03; 						//�̶���ͷ
		head4[1] = 0x00; 						//�̶���ͷ
		head4[2] = strlen(humiture_buff1);  	//ʣ�೤��	
			
		// �����������
		sprintf(tempAll2, "%c%c%c%s", head2[0], head2[1], head2[2], temp2);   // tempAll2 ���������� + �̶���ͷ
		//������ʾ�������
		u1_printf("\r\n");   // ��ӡ�س�
		u1_printf("%s\r\n", tempAll2 + 3);   // ��ӡҪ���͵�����
	  // ����������
		sprintf(tempAll3, "%c%c%c%s", head3[0], head3[1], head3[2], temp3);
		//������ʾ�������
		u1_printf("\r\n");   // ��ӡ�س�
		u1_printf("%s\r\n", tempAll3 + 3);   // ��ӡҪ���͵�����
		// �����ʪ������
		sprintf(tempAll4, "%c%c%c%s", head4[0], head4[1], head4[2], humiture_buff1);
		//������ʾ�������
		u1_printf("\r\n");    // ��ӡ�س�
		u1_printf("%s\r\n", tempAll4 + 3);   // ��ӡҪ���͵�����

//		dataLen1 = strlen(temp1)+ 3;
    // �������ݳ���
		dataLen2 = strlen(temp2)+ 3;
		// mqtt �������ݸ�������
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll2, dataLen2);//������ݣ�������������
		dataLen3 = strlen(temp3)+ 3;
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll3, dataLen3);//������ݣ�������������
		dataLen4 = strlen(humiture_buff1)+ 3;
		MQTT_PublishQs0(Data_TOPIC_NAME,tempAll4, dataLen4);//������ݣ�������������
    // ���һЩ�жϱ�־λ

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   	
	}
}

/*-------------------------------------------------*/
/*�����������������жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ӳ������������жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*���������ڴ�����жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������Ԥȡָʧ�ܣ��洢������ʧ���жϴ�����   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*��������δ�����ָ���Ƿ�״̬������           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{

}

/*-------------------------------------------------*/
/*�����������жϣ�SWI ָ����õĴ�����           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*�����������Լ����������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
	
}

/*-------------------------------------------------*/
/*���������ɹ����ϵͳ��������                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������SysTicϵͳ��શ�ʱ��������             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
 
}
/*-------------------------------------------------*/
/*��������GPS DMA�жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DMA1_Channel3_IRQHandler(void)
{
  
  GPS_ProcessDMAIRQ();

}
