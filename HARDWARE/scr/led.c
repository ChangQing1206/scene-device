 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED（补光灯）控制           	    */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PC13  LED0

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化LED函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
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
/*函数名：LED开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
			GPIO_ResetBits(GPIOC,GPIO_Pin_13); 						 //PC13输出低
} 


/*-------------------------------------------------*/
/*函数名：LED关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
			GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PC13 输出高
}



