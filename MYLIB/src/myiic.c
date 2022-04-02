#include "myiic.h"
#include "SHT30.h"
#include "delay.h"
#include "stm32f10x.h"

//from 正点原子

/************************************************************************
*
*    函 数 名：IIC_Init
*    函数功能：初始化模拟IIC需用到的两个IO口，其中PB9为IIC_SDA，PB8为IIC_CLK。
*    输入参数：void
*    输出参数：void
*    返 回 值：void
*
*    作    者：Guo MingHong
*    创建时间：2021-02-05
*    其他说明：使用该函数可以直接初始化IO口，不用在cube里面配置。（cube不支持软件iic所以就只能这样啦）
*    修改信息：无
*
************************************************************************/
void IIC_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;            //准备设置PB8.PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //复用推挽输出，用于串口1的发送
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB8.PB9
    
	
    IIC_SDA = 1;
    IIC_SCL = 1;  
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //SDA设置成输出模式
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	DelayUs(4);
 	IIC_SDA = 0;	//IIC起始信号:when CLK is high,DATA change form high to low 
	DelayUs(4);
	IIC_SCL=0;		//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();		//SDA设置成输出模式
	IIC_SCL=0;
	IIC_SDA=0;		//IIC停止信号:when CLK is high DATA change form low to high
 	DelayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;		//发送I2C总线结束信号
	DelayUs(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();    //SDA设置成输出模式
	IIC_SDA=1;
	DelayUs(1);	   
	IIC_SCL=1;
	DelayUs(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)//通信超时时间
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;//SCL电平拉低 
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();	//SDA设置成输出模式
	IIC_SDA=0;
	DelayUs(2);
	IIC_SCL=1;
	DelayUs(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();	//SDA设置成输出模式
	IIC_SDA=1;
	DelayUs(2);
	IIC_SCL=1;
	DelayUs(2);
	IIC_SCL=0;
}	

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_OUT(); 		//SDA设置成输出模式   
	IIC_SCL = 0;	//拉低SCL开始数据传输
	for(t=0;t<8;t++)
	{              
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1; 	  
		DelayUs(2);//对TEA5767这三个延时都是必须的，TEA5767为飞利浦的一款音频芯片，这里的延时可加可不加
		IIC_SCL = 1;
		DelayUs(2); 
		IIC_SCL = 0;	
		DelayUs(2);
	}	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();		//SDA设置为输入模式
  for(i=0; i<8; i++ )
  {
		IIC_SCL = 0; 
		DelayUs(2);
		IIC_SCL = 1;
		receive <<= 1;
		if(READ_SDA)
		{
			receive++; 
		}  
		DelayUs(1); 
  }					 
  if(!ack)
	{
	  IIC_NAck();//发送nACK
	}       
	else
	{
		IIC_Ack(); //发送ACK  
	}		 
	return receive;
}


