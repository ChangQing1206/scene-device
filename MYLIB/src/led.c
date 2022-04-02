 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED（补光灯）控制           	    */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PC13  LED0
// PB3   LED1
// PB4   LED2

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED0函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
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
/*函数名：LED0开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED0_On(void)
{			
			GPIO_ResetBits(GPIOC,GPIO_Pin_13); 						 //PC13输出低
} 


/*-------------------------------------------------*/
/*函数名：LED0关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED0_Off(void)
{		
			GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PC13 输出高
}

/*-------------------------------------------------*/
/*函数名：初始化LED1函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
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
/*函数名：LED1开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED1_On(void)
{			
			GPIO_ResetBits(GPIOB,GPIO_Pin_3); 						 //PC13输出低
} 


/*-------------------------------------------------*/
/*函数名：LED1关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED1_Off(void)
{		
			GPIO_SetBits(GPIOB,GPIO_Pin_3);						 //PC13 输出高
}

/*-------------------------------------------------*/
/*函数名：初始化LED2函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
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
/*函数名：LED2开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED2_On(void)
{			
			GPIO_ResetBits(GPIOB,GPIO_Pin_4); 						 //PC13输出低
} 


/*-------------------------------------------------*/
/*函数名：LED2关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED2_Off(void)
{		
			GPIO_SetBits(GPIOB,GPIO_Pin_4);						 //PC13 输出高
}



