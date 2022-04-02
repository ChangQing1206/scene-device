#include "stm32f10x.h"  //包含需要的头文件
#include "usart3.h"     //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化串口2发送功能                        */
/*参  数：bound：波特率                             */
/*返回值：无                                        */
/*-------------------------------------------------*/
void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
		USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
		NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3	
      
	  // 串口3 与 GPS
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口3时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
		USART_DeInit(USART3);                                  //串口3寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //准备设置PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口3的发送
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //准备设置PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口2的接收
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB11
	
		USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
      
    USART_Init(USART2, &USART_InitStructure);                                      //设置串口2	

		USART_ClearFlag(USART2, USART_FLAG_RXNE);	              //清除接收标志位
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //开启接收中断
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //设置串口2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //中断通道使能
		NVIC_Init(&NVIC_InitStructure);	                          //设置串口2中断 

		USART_Cmd(USART2, ENABLE);                                //使能串口2
}
