#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart3.h"     //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ������2���͹���                        */
/*��  ����bound��������                             */
/*����ֵ����                                        */
/*-------------------------------------------------*/
void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
		USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
		NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3	
      
	  // ����3 �� GPS
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ�ܴ���3ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
		USART_DeInit(USART3);                                  //����3�Ĵ�����������ΪĬ��ֵ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //׼������PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���3�ķ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //׼������PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���2�Ľ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PB11
	
		USART_InitStructure.USART_BaudRate = bound;                                    //����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
      
    USART_Init(USART2, &USART_InitStructure);                                      //���ô���2	

		USART_ClearFlag(USART2, USART_FLAG_RXNE);	              //������ձ�־λ
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //���������ж�
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //���ô���2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //�ж�ͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	                          //���ô���2�ж� 

		USART_Cmd(USART2, ENABLE);                                //ʹ�ܴ���2
}
