#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

//from ����ԭ��


#define	write	0
#define	read	1
//IO��������
#define SDA_IN()  {GPIOB->CRH &= 0XFFFFFF0F; GPIOB->CRH |= 8<<1*4;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->CRH &= 0XFFFFFF0F; GPIOB->CRH |= 3<<1*4;} //PB9���ģʽ
//IO����
#define IIC_SCL   PBout(8) //SCL
#define IIC_SDA   PBout(9) //SDA
#define READ_SDA  PBin(9)  //����SDA

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 

#endif

