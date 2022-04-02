 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED������ƣ�����           	    */
/*                                                 */
/*-------------------------------------------------*/

// Ӳ�����ӣ�
// PC13  LED0
// PB3   LED1
// PB4   LED2

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��LED0����                       	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED0_Init(void)
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
/*��������LED0����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED0_On(void)
{			
			GPIO_ResetBits(GPIOC,GPIO_Pin_13); 						 //PC13�����
} 


/*-------------------------------------------------*/
/*��������LED0�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED0_Off(void)
{		
			GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PC13 �����
}

/*-------------------------------------------------*/
/*����������ʼ��LED1����                       	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED1_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
}

/*-------------------------------------------------*/
/*��������LED1����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED1_On(void)
{			
			GPIO_ResetBits(GPIOB,GPIO_Pin_3); 						 //PC13�����
} 


/*-------------------------------------------------*/
/*��������LED1�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED1_Off(void)
{		
			GPIO_SetBits(GPIOB,GPIO_Pin_3);						 //PC13 �����
}

/*-------------------------------------------------*/
/*����������ʼ��LED2����                       	    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED2_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	
}

/*-------------------------------------------------*/
/*��������LED2����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED2_On(void)
{			
			GPIO_ResetBits(GPIOB,GPIO_Pin_4); 						 //PC13�����
} 


/*-------------------------------------------------*/
/*��������LED2�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED2_Off(void)
{		
			GPIO_SetBits(GPIOB,GPIO_Pin_4);						 //PC13 �����
}



