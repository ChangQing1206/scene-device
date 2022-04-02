
/*-------------------------------------------------*/
/*                                                 */
/*          		��ʱ��2                       */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h" 			 //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʱ��2ʹ��10s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_ENABLE_10S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;             //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                           //����һ�������жϵı���	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //ʹ��TIM2ʱ��	
	TIM_DeInit(TIM2);                                              //��ʱ��2�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	           //�����Զ���װ��ֵ0.2s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36000-1;             //���ö�ʱ��Ԥ��Ƶ��  3600    36000/72 * 60000 / 1000000 = 30s
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //1��Ƶ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);            //����TIM2
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                     //ʹ��TIM2����ж�    
	TIM_Cmd(TIM2, ENABLE);                                         //��TIM2                          
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                //����TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;             //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                //�����ж�
}