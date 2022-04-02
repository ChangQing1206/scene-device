/*-------------------------------------------------*/
/*                                                 */
/*                 ���ƶ˿�           		   	   */
/*                                                 */
/*-------------------------------------------------*/

//LED�������ݷ��͸�������

#include "stm32f10x.h"
#include "control.h"
#include "usart1.h"
#include "mqtt.h"

extern char *ledFlag;               //�����״̬                   

/*-------------------------------------------------*/
/*�����������ݳ��� length ����                      */
/*��  ������                                       */
/*����ֵ������                 				       */
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
/*�����������Ϳ����豸����                          */
/*��  ������                                       */
/*����ֵ��							               */
/*-------------------------------------------------*/
void Send_Data(void)
{		
/*-------------------------------------------------*/

	char  head2[3];
	char  temp2[50];          	//����һ����ʱ������1,��������ͷ
	char  tempAll2[100];       	//����һ����ʱ������2��������������
	char  head3[3];
	char  temp3[50];          	//����һ����ʱ������1,��������ͷ
	char  tempAll3[100];       	//����һ����ʱ������2��������������
	char  head4[3];
	char  humiture_buff1[100];    //����һ����ʱ������1,��������ͷ
	char  tempAll4[150];       	//����һ����ʱ������2��������������

//	//u16  ms;

	int   dataLen2;     	  	    //���ĳ���	
	int   dataLen3;     	  	    //���ĳ���	
	int   dataLen4;     	  	    //���ĳ���
	

	memset(temp2,       0, 50);  //��ջ�����1
	memset(tempAll2,    0, 100); //��ջ�����2
	memset(head2,       0, 3);   //���MQTTͷ 
	memset(temp3,       0, 50);  //��ջ�����1
	memset(tempAll3,    0, 100); //��ջ�����2
	memset(head3,       0, 3);   //���MQTTͷ 
	memset(humiture_buff1,0, 100);  //��ջ�����1
	memset(tempAll4,    0, 150); //��ջ�����2
	memset(head4,       0, 3);   //���MQTTͷ    	
	

	sprintf(temp2, "{\"ledFlag_mq7\":\"%s\"}", ledFlag);//��������
	sprintf(temp3, "{\"ledFlag_light\":\"%s\"}", ledFlag);//��������
	sprintf(humiture_buff1, "{\"ledFlag\":\"%s\"}", ledFlag);//��������
	
	//sprintf(temp, "hello");//��������

	head2[0] = 0x03; //�̶���ͷ
	head2[1] = 0x00; //�̶���ͷ
	head2[2] = strlen(temp2); //ʣ�೤��	
	sprintf(tempAll2, "%c%c%c%s", head2[0], head2[1], head2[2], temp2);
	head3[0] = 0x03; //�̶���ͷ
	head3[1] = 0x00; //�̶���ͷ
	head3[2] = strlen(temp3); //ʣ�೤��	
	sprintf(tempAll3, "%c%c%c%s", head3[0], head3[1], head3[2], temp3);
	head4[0] = 0x03; //�̶���ͷ
	head4[1] = 0x00; //�̶���ͷ
	head4[2] = strlen(humiture_buff1); //ʣ�೤��	
	sprintf(tempAll4, "%c%c%c%s", head4[0], head4[1], head4[2], humiture_buff1);
	
	//ms = 10000;


	dataLen2 = strlen(temp2) + 3;
	u1_printf("all: %s\r\n", tempAll2+3);
	dataLen3 = strlen(temp3) + 3;
	u1_printf("all: %s\r\n", tempAll3+3);
	dataLen4 = strlen(humiture_buff1) + 3;
	u1_printf("all: %s\r\n", tempAll4+3);
	
	//u1_printf("�ܳ��ȣ�%d\r\n",Init_len);
	//DelayMs(ms);

	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll2, dataLen2); //������ݣ�����������
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll3, dataLen3); //������ݣ�����������
	MQTT_PublishQs0(Data_TOPIC_NAME, tempAll4, dataLen4); //������ݣ���������
	//�ⲿ�����ص㣬��ô�������ݷ��͵�topic��Ŀǰֱ�ӵ������溯�����Է��͹̶���ͷ��QT�����������ݲ�û�з��͹�ȥ
	
	
	
	
	
	
	
	
}	
