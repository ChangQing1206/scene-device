 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED������ƣ�����           	    */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PC13  LED0

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED����                       	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
}

/*-------------------------------------------------*/
/*��������LED����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
			GPIO_ResetBits(GPIOC,GPIO_Pin_13); 						 //PC13�����
} 


/*-------------------------------------------------*/
/*��������LED�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
			GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PC13 �����
}



