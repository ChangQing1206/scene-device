/**
  ******************************************************************************
  * @file    gps_config.c
  * @author  fire
  * @version V1.0
  * @date    2016-07-xx
  * @brief   gpsģ��ӿ���������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
	*/

#include "gps_config.h"
#include "nmea/nmea.h"
#include "usart1.h"
#include "stdio.h"
//#include "./usart/bsp_usart.h"



/********************************************���õ����ݣ��������Ƿϵ�************************************/
//���徭γ�ȱ���  ȫ�ֱ���
double longtitude;
double latitude;
char temp_gpsdata[73]="";//һ�����ڽ��գ�һ�����ڴ�����ֹ���ݱ����ǣ�
char gps_data[73]="$GPGLL,2160.7668,N,11027.7624,E,092321.00,A,A*60";
char i = 0;//���ڼ���
char USART_RX_OK_FLAG = 0;	//����һ֡������ɵı�־
/********************************************���õ����ݣ��������Ƿϵ�************************************/
/* DMA���ջ���  */
uint8_t gps_rbuff[GPS_RBUFF_SIZE];

/* DMA���������־ */
__IO uint8_t GPS_TransferEnd = 0, GPS_HalfTransferEnd = 0;


/*-------------------------------------------------*/
/*��������printf�ض���      				      	   */
/*��  ����                                         */
/*����ֵ��                                         */
/*-------------------------------------------------*/  


/*********************************************���õ�����,�������Ƿϵ�**********************************************/
//�ú����� bug ����Ҫ�����ݵ���Ч�Խ����жϣ���Ч���ز�
//��ȡ�����GPS��Ϣ
void GET_GPS(void)
{
	u1_printf("ssss");
	USART_Cmd(GPS_USART, ENABLE);  //ʹ�ܴ���3����GPS��Ϣ
	while(1)
	{
		// �жϸı�
		if(USART_RX_OK_FLAG)//�ж��Ƿ������һ֡GPS����
		{
			USART_RX_OK_FLAG = 0;	//���������ɱ�־��
			strcpy(gps_data,temp_gpsdata);//�������յ�������
			// ��γ��
			GPS_data_process();

			u1_printf("\r\ngps data %s\r\n",gps_data);
			if(strstr(gps_data,"$GNGLL") != NULL)
			{
				// ���������ɣ��˳�ѭ��
					break;
			}
		}
	}
	USART_Cmd(GPS_USART, DISABLE);  // �رմ���
}

/*

,,17,33,289,30,19,17,270,*77
$GPGSV,3,3,12,21,29,038,,30,58,251,,194,62,109,33,195,60,046,27*7D
$BDGSV,1,1,02,20,09,116,27,40,53,028,25*6D
$GNRMC,065111.000,A,2235.83852,N,11304.84201,E,1.21,0.00,020422,,,E*78
$GNVTG,0.00,T,,M,1.21,N,2.25,K,E*20
$GNZDA,065111.000,02,04,2022,00,00*4E
$GPTXT,01,01,01,ANTENNA OK*35
$GNGGA,065112.000,2235.83852,N,11304.84201,E,6,07,3.0,50.2,M,0.0,M,,*4D
$GNGLL,2235.83852,N,11304.84201,E,065112.000,A,E*48
$GPGSA,A,3,01,03,17,194,195,,,,,,,,4.7,3.0,3.7*33
$BDGSA,A,3,20,40,,,,,,,,,,,4.7,3.0,3.7*21
$GPGSV,3,1,12,01,63,038,27,03,28,130,24,06,10,216,,07,51,204,*78
$GPGSV,3,2,12,08,14,058,,14,42,332,,17,33,289,30,19,17,270,*77
$GPGSV,3,3,12,21,29,038,,30,58,251,,194,62,109,33,195,60,046,27*7D
$BDGSV,1,1,02,20,09,116,27,40,53,028,25*6D
$GNRMC,065112.000,A,2235.83852,N,11304.84201,E,1.21,0.00,020422,,,E*7B
$GNVTG,0.00,T,,M,1.21,N,2.25,K,E*20
$GNZDA,065112.000,02,04,2022,00,00*4D
$GPTXT,01,01,01,ANTENNA OK*35
$GNGGA,065113.000,2235.83852,N,11304.84201,E,6,07,3.0,50.2,M,0.0,M,,*4C
$GNGLL,2235.83852,N,11304.84201,E,065113.000,A,E*49
$GPGSA,A,3,01,03,17,194,195,,,,,,,,4.7,3.0,3.7*33
$BDGSA,A,3,20,40,,,,,,,,,,,4.7,3.0,3.7*21
$GPGSV,3,1,12,01,63,038,27,03,28,130,24,06,10,216,,07,51,204,*78
$GPGSV,3,2,12,08,14,058,,14,42,332,,17,33,289,30,19,17,270,*77
$GPGSV,3,3,12,21,29,038,,30,58,251,,194,62,109,33,195,60,046,27*7D
$BDGSV,1,1,02,20,09,116,28,40,53,028,25*62
$GNRMC,065113.000,A,2235.83852,N,11304.84201,E,1.21,0.00,020422,,,E*7A
$GNVTG,0.00,T,,M,1.21,N,2.25,K,E*20
$GNZDA,065113.000,02,04,2022,00,00*4C
$GPTXT,01,01,01,ANTENNA OK*35
$GNGGA,065113.980,2235.82980,N,11304.83754,E,1,07,3.0,50.3,M,0.0,M,,*46
$GNGLL,2235.82980,N,11304.83754,E,065113.980,A,A*41
$GPGSA,A,3,01,03,17,194,195,,,,,,,,4.7,3.0,3.7*33
$BDGSA,A,3,20,40,,,,,,,,,,,4.7,3.0,3.7*21
$GPGSV,3,1,12,01,63,038,27,03,28,130,25,06,10,216,,07,51,204,*79
$GPGSV,3,2,12,08,14,058,,14,42,332,,17,33,289,30,19,17,270,*77
$GPGSV,3,3,12,21,29,038,,30,58,251,,194,62,109,33,195,60,046,27*7D
$BDGSV,1,1,02,20,09,116,28,40,53,028,25*62
$GNRMC,065113.980,A,2235.82980,N,11304.83754,E,0.45,0.00,020422,,,A*71
$GNVTG,0.00,T,,M,0.45,N,0.83,K,A*29
$GNZDA,065113.980,02,04,2022,00,00*4D
$GPTXT,01,01,01,ANTENNA OK*35


*/

// $GNGLL,2235.84519,N,11304.85594,E,070135.000,A,A*42

//GPS��Ϣ����ȡ
void GPS_data_process()
{
	unsigned char i = 1;
	unsigned char j = 0;
	char num_size[7];
	char str_num[7][15];
	float num[3];
	char str[4];
	if(strstr(gps_data,"$GNGLL") != NULL )	//�����ж����ݵ���Ч��
	{
		for(i = 1;i < 7;i++)
		{
			num_size[i-1] = ((NMAE_find_pos(i+1) - NMAE_find_pos(i)) -1 );	//��������������֮���ж��ٸ��ַ�
			strncpy(str_num[i-1],&gps_data[NMAE_find_pos(i)+1],num_size[i-1]); // GNGLL   2235.84519   N    11304.85594    E    070135.000    A     A*42
		}
		
		num_size[i-1] = 1;
		str_num[i-1][0] = gps_data[NMAE_find_pos(i)+1];
		
		
		for(i = 0;i <= 4;i=i+2)
		{
			num[j++] = NMAE_str2num(&str_num[i][0]);
		}
		
		j = 0;
		
		for(i = 1;i <= 5;i=i+2)
		{
			str[j++] = *str_num[i];
		}
		str[j] = *str_num[6];	
    
    // ȡ�þ�γ��
    if(str[0] == 'S') {
       num[0] = - num[0];
    }
    if(str[1] == 'W') {
       num[1] = - num[1];
    }
    longtitude = num[1];
    latitude = num[0];		
	}
}




/*
*	�� gps_data[] �ҵ��� num �� ���������±겢����
*
*/
unsigned char NMAE_find_pos(unsigned char num)
{
	unsigned char i = 0;
	while(num)
	{
		if(gps_data[i] == '*' || gps_data[i] < ' ' || gps_data[i] > 'z')
			return 0x00;
		
		if(gps_data[i] == ',')
			num--;
		i++;
	}
	return --i;
}

/*
*	�� buf �ַ���ת�������֣���ȷ���֣�ʡ��С����
*
*/
float NMAE_str2num(char *buf)
{
	signed char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	int integer = 0;
	float dec = 0;
	float result = 0;
	char *p = buf;
	
	while(1)//�������ֵĳ���
	{
		if((*p > '9') | (*p < '0'))
		{
			if(*p == '.')
				i = k;
			else
				break;
		}
		p++;
		k++;
	}

	j = i + 1;
	i--;
	p = buf;
	
	while(i >= 0)
	{
		integer += ((*p-'0') * num_pow(10,i));
		i--;
		p++;
	}
	
	for(i=1;j<k;j++)
	{
		p++;
		dec += (*p - '0') / (num_pow(10,i) / 1.0);
		i++;
	}
	
	result = (float)(integer/100) + (((float)(integer%100) + dec) / 60.0);
	return result;
	
}

/*
*	���� num1 �� num2 �η����� num1^num2
*
*/
int num_pow(int num1,int num2)
{
	int result = 1;
	while(num2)
	{
		result *= num1;
		num2--;
	}
	
	return result;
}
/**
  * @brief  GPS_Interrupt_Config ����GPSʹ�õ�DMA�ж� 
  * @param  None.
  * @retval None.
  */
static void GPS_Interrupt_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// DMA2 Channel Interrupt ENABLE
	NVIC_InitStructure.NVIC_IRQChannel = GPS_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/*
GPS
GND  <---->  GND
3.3V <---->  VCC
PB10 <---->  RXD
PB11 <---->  TXD

I.ch340 <----> stm32    (����1ͨ�� ���ڵ�����ʾ)
PA9 <----> RXD
PA10 <----> TXD
*/

void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3	
      
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ�ܴ���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
	USART_DeInit(USART3);                                  //����3�Ĵ�����������ΪĬ��ֵ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //׼������PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���2�ķ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //׼������PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���3�Ľ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //����PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
      
    USART_Init(USART3, &USART_InitStructure);                                      //���ô���3	

	USART_ClearFlag(USART3, USART_FLAG_RXNE);	              //������ձ�־λ
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //���ô���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                          //���ô���3�ж� 

//	USART_Cmd(USART3, ENABLE);                                //ʹ�ܴ���3
}

/*
 * ��������GPS_USART_Config   ʹ��PB10 PB11 ���Ǵ���3��GPSͨ��
 * ����  ��USART GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
static void GPS_USART_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  
	/* config USART clock */
  // ʹ��GPIOʱ��
//	RCC_AHBPeriphClockCmd(GPS_DMA_CLK, ENABLE);
	// ʹ�ܴ���ʱ��
	
	RCC_APB2PeriphClockCmd(GPS_USART_RX_GPIO_CLK|GPS_USART_TX_GPIO_CLK, ENABLE);
  
	GPS_RCC_PeriphClockCmd_Fun(GPS_USART_CLK, ENABLE);
	u1_printf("hello2");
	/* USART GPIO config */
  /* Configure USART Tx  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPS_USART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPS_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	    
  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPS_USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPS_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	u1_printf("hello3");  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = GPS_USART_BAUDRATE;                //GPSģ��Ĭ��ʹ�ò����ʣ�9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(GPS_USART, &USART_InitStructure); 
	// �ж�����
  GPS_Interrupt_Config();
//	USART_Cmd(GPS_USART, ENABLE);	
	u1_printf("hello4");
	
}

/**
  * @brief  GPS_Config gps ��ʼ��
  * @param  ��
  * @retval ��
  */
void GPS_Config(void)
{
//  GPS_USART_Config();
	Usart3_Init(9600);
//  GPS_DMA_Config(); 
//  decode_init();
}


/*********************************************���õ�����,�������Ƿϵ�**********************************************/

/**
  * @brief  GPS_DMA_Config gps dma��������
  * @param  ��
  * @retval ��
  */
static void GPS_DMA_Config(void)
{
   DMA_InitTypeDef DMA_InitStructure;
	 u1_printf("hello5");
	
		/*����DMAʱ��*/
		RCC_AHBPeriphClockCmd(GPS_DMA_CLK, ENABLE);

		/*����DMAԴ���������ݼĴ�����ַ*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = GPS_DATA_ADDR;	   

		/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gps_rbuff;

		/*���򣺴��ڴ浽����*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	

		/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = GPS_RBUFF_SIZE;

		/*�����ַ����*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*�ڴ��ַ����*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*�������ݵ�λ*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*�ڴ����ݵ�λ 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMAģʽ������ѭ��*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*���ȼ�����*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*����DMA��ͨ��*/		   
		DMA_Init(GPS_DMA_CHANNEL, &DMA_InitStructure); 	   
    
//    GPS_Interrupt_Config();
		
    DMA_ITConfig(GPS_DMA_CHANNEL,DMA_IT_HT|DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�

		/*ʹ��DMA*/
//		DMA_Cmd (GPS_DMA_CHANNEL,ENABLE);		
    
    /* ���ô��� �� DMA����TX���� */
//		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, ENABLE);
		u1_printf("hello6");
}



nmeaINFO info;          //GPS�����õ�����Ϣ
nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
uint8_t new_parse;    //�Ƿ����µĽ������ݱ�־

nmeaTIME beiJingTime;    //����ʱ�� 

// ������ʼ��gps�������ݽ⹹ ��ֹ�����������
void decode_init(void) {
//	  nmeaINFO info;          //GPS�����õ�����Ϣ
//    nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
//    uint8_t new_parse=0;    //�Ƿ����µĽ������ݱ�־
//  
//    nmeaTIME beiJingTime;    //����ʱ�� 
	  new_parse = 0;

    /* �����������������Ϣ�ĺ��� */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;

    /* ��ʼ��GPS���ݽṹ */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
}

/**
  * @brief  GPS_ProcessDMAIRQ GPS DMA�жϷ�����
  * @param  None.
  * @retval None.
  */
void GPS_ProcessDMAIRQ(void)
{  
	u1_printf("hello9999");
  if(DMA_GetITStatus(GPS_DMA_IT_HT) )         /* DMA �봫����� */
  {
    GPS_HalfTransferEnd = 1;                //���ð봫����ɱ�־λ
    DMA_ClearFlag(GPS_DMA_FLAG_HT);
  }
  else if(DMA_GetITStatus(GPS_DMA_IT_TC))     /* DMA ������� */
  {
    GPS_TransferEnd = 1;                    //���ô�����ɱ�־λ
    DMA_ClearFlag(GPS_DMA_FLAG_TC);

   }
}



/**
  * @brief  trace �ڽ���ʱ��������GPS���
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void trace(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    //��gps_config.h�ļ���������꣬�Ƿ����������Ϣ
    uint16_t i;
    u1_printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      u1_printf("%c",*(str+i));
  
    u1_printf("\n");
  #endif
}

/**
  * @brief  error �ڽ������ʱ�����ʾ��Ϣ
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void error(const char *str, int str_size)
{
    #ifdef __GPS_DEBUG   //��gps_config.h�ļ���������꣬�Ƿ����������Ϣ

    uint16_t i;
    u1_printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      u1_printf("%c",*(str+i));
    u1_printf("\n");
    #endif
}

/**
  * @brief  error �ڽ������ʱ�����ʾ��Ϣ
  * @param  str: Ҫ������ַ�����str_size:���ݳ���
  * @retval ��
  */
void gps_info(const char *str, int str_size)
{

    uint16_t i;
    u1_printf("\r\nInfo: ");
    for(i=0;i<str_size;i++)
      u1_printf("%c",*(str+i));
    u1_printf("\n");
}



/******************************************************************************************************** 
**     ��������:            bit        IsLeapYear(uint8_t    iYear) 
**    ��������:            �ж�����(�������2000�Ժ�����) 
**    ��ڲ�����            iYear    ��λ���� 
**    ���ڲ���:            uint8_t        1:Ϊ����    0:Ϊƽ�� 
********************************************************************************************************/ 
static uint8_t IsLeapYear(uint8_t iYear) 
{ 
    uint16_t    Year; 
    Year    =    2000+iYear; 
    if((Year&3)==0) 
    { 
        return ((Year%400==0) || (Year%100!=0)); 
    } 
     return 0; 
} 

/******************************************************************************************************** 
**     ��������:            void    GMTconvert(uint8_t *DT,uint8_t GMT,uint8_t AREA) 
**    ��������:            ��������ʱ�任�������ʱ��ʱ�� 
**    ��ڲ�����            *DT:    ��ʾ����ʱ������� ��ʽ YY,MM,DD,HH,MM,SS 
**                        GMT:    ʱ���� 
**                        AREA:    1(+)���� W0(-)���� 
********************************************************************************************************/ 
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //������ʱ�����ݴ���� 
     
    if(GMT==0)    return;                //�������0ʱ��ֱ�ӷ��� 
    if(GMT>12)    return;                //ʱ�����Ϊ12 �����򷵻�         

    YY    =    SourceTime->year;                //��ȡ�� 
    MM    =    SourceTime->mon;                 //��ȡ�� 
    DD    =    SourceTime->day;                 //��ȡ�� 
    hh    =    SourceTime->hour;                //��ȡʱ 
    mm    =    SourceTime->min;                 //��ȡ�� 
    ss    =    SourceTime->sec;                 //��ȡ�� 

    if(AREA)                        //��(+)ʱ������ 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+GMT-24;        //�ȵó�ʱ�� 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //���·�(12�µ�������) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //С�·�2�µ�������) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //����2�·� 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //��������������2��29�� ���߲�����������2��28�� 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //����12�·� 
            { 
                if(DD<31)    DD++; 
                else        //�������һ�� 
                {               
                    DD    =    1; 
                    MM    =    1; 
                    YY    ++; 
                } 
            } 
        } 
    } 
    else 
    {     
        if(hh>=GMT)    hh    -=    GMT;    //������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+24-GMT;        //�ȵó�ʱ�� 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //�����Ǵ��·�(1�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //������С�·�2�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //�����ϸ�����2�·� 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //�������� 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //����1�·� 
            { 
                if(DD>1)    DD--; 
                else        //�����һ�� 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //������ 
    ConvertTime->mon    =    MM;                //������ 
    ConvertTime->day    =    DD;                //������ 
    ConvertTime->hour   =    hh;                //����ʱ 
    ConvertTime->min    =    mm;                //���·� 
    ConvertTime->sec    =    ss;                //������ 
}  






/*********************************************************end of file**************************************************/
