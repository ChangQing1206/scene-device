#include "myiic.h"
#include "SHT30.h"
#include "delay.h"
#include "stm32f10x.h"

//from ����ԭ��

/************************************************************************
*
*    �� �� ����IIC_Init
*    �������ܣ���ʼ��ģ��IIC���õ�������IO�ڣ�����PB9ΪIIC_SDA��PB8ΪIIC_CLK��
*    ���������void
*    ���������void
*    �� �� ֵ��void
*
*    ��    �ߣ�Guo MingHong
*    ����ʱ�䣺2021-02-05
*    ����˵����ʹ�øú�������ֱ�ӳ�ʼ��IO�ڣ�������cube�������á���cube��֧�����iic���Ծ�ֻ����������
*    �޸���Ϣ����
*
************************************************************************/
void IIC_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;            //׼������PB8.PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	       //����������������ڴ���1�ķ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PB8.PB9
    
	
    IIC_SDA = 1;
    IIC_SCL = 1;  
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //SDA���ó����ģʽ
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	DelayUs(4);
 	IIC_SDA = 0;	//IIC��ʼ�ź�:when CLK is high,DATA change form high to low 
	DelayUs(4);
	IIC_SCL=0;		//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();		//SDA���ó����ģʽ
	IIC_SCL=0;
	IIC_SDA=0;		//IICֹͣ�ź�:when CLK is high DATA change form low to high
 	DelayUs(4);
	IIC_SCL=1; 
	IIC_SDA=1;		//����I2C���߽����ź�
	DelayUs(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();    //SDA���ó����ģʽ
	IIC_SDA=1;
	DelayUs(1);	   
	IIC_SCL=1;
	DelayUs(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)//ͨ�ų�ʱʱ��
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;//SCL��ƽ���� 
	return 0;  
} 

//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();	//SDA���ó����ģʽ
	IIC_SDA=0;
	DelayUs(2);
	IIC_SCL=1;
	DelayUs(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();	//SDA���ó����ģʽ
	IIC_SDA=1;
	DelayUs(2);
	IIC_SCL=1;
	DelayUs(2);
	IIC_SCL=0;
}	

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_OUT(); 		//SDA���ó����ģʽ   
	IIC_SCL = 0;	//����SCL��ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1; 	  
		DelayUs(2);//��TEA5767��������ʱ���Ǳ���ģ�TEA5767Ϊ�����ֵ�һ����ƵоƬ���������ʱ�ɼӿɲ���
		IIC_SCL = 1;
		DelayUs(2); 
		IIC_SCL = 0;	
		DelayUs(2);
	}	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();		//SDA����Ϊ����ģʽ
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
	  IIC_NAck();//����nACK
	}       
	else
	{
		IIC_Ack(); //����ACK  
	}		 
	return receive;
}


