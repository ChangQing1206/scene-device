/**
  ******************************************************************************
  * @file    gps_config.c
  * @author  fire
  * @version V1.0
  * @date    2016-07-xx
  * @brief   gps模块接口配置驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
	*/

#include "gps_config.h"
#include "nmea/nmea.h"
#include "usart1.h"
#include "stdio.h"
//#include "./usart/bsp_usart.h"



/********************************************有用的数据，其他都是废的************************************/
//定义经纬度变量  全局变量
double longtitude;
double latitude;
char temp_gpsdata[73]="";//一个用于接收，一个用于处理，防止数据被覆盖；
char gps_data[73]="$GPGLL,2160.7668,N,11027.7624,E,092321.00,A,A*60";
char i = 0;//串口计数
char USART_RX_OK_FLAG = 0;	//接收一帧数据完成的标志
/********************************************有用的数据，其他都是废的************************************/
/* DMA接收缓冲  */
uint8_t gps_rbuff[GPS_RBUFF_SIZE];

/* DMA传输结束标志 */
__IO uint8_t GPS_TransferEnd = 0, GPS_HalfTransferEnd = 0;


/*-------------------------------------------------*/
/*函数名：printf重定向      				      	   */
/*参  数：                                         */
/*返回值：                                         */
/*-------------------------------------------------*/  


/*********************************************有用的设置,其他都是废的**********************************************/
//该函数的 bug ，需要对数据的有效性进行判断，无效就重测
//获取自身的GPS信息
void GET_GPS(void)
{
	u1_printf("ssss");
	USART_Cmd(GPS_USART, ENABLE);  //使能串口3接收GPS信息
	while(1)
	{
		// 中断改变
		if(USART_RX_OK_FLAG)//判断是否接收完一帧GPS数据
		{
			USART_RX_OK_FLAG = 0;	//清除接收完成标志；
			strcpy(gps_data,temp_gpsdata);//拷贝接收到的数据
			// 经纬度
			GPS_data_process();

			u1_printf("\r\ngps data %s\r\n",gps_data);
			if(strstr(gps_data,"$GNGLL") != NULL)
			{
				// 如果拷贝完成，退出循环
					break;
			}
		}
	}
	USART_Cmd(GPS_USART, DISABLE);  // 关闭串口
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

//GPS信息的提取
void GPS_data_process()
{
	unsigned char i = 1;
	unsigned char j = 0;
	char num_size[7];
	char str_num[7][15];
	float num[3];
	char str[4];
	if(strstr(gps_data,"$GNGLL") != NULL )	//粗略判断数据的有效性
	{
		for(i = 1;i < 7;i++)
		{
			num_size[i-1] = ((NMAE_find_pos(i+1) - NMAE_find_pos(i)) -1 );	//计算两个‘，’之间有多少个字符
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
    
    // 取得经纬度
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
*	在 gps_data[] 找到第 num 个 ‘，’的下标并返回
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
*	将 buf 字符串转换成数字，精确到分，省略小数点
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
	
	while(1)//计算数字的长度
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
*	计算 num1 的 num2 次方，即 num1^num2
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
  * @brief  GPS_Interrupt_Config 配置GPS使用的DMA中断 
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

I.ch340 <----> stm32    (串口1通信 用于调试显示)
PA9 <----> RXD
PA10 <----> TXD
*/

void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3	
      
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能串口3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
	USART_DeInit(USART3);                                  //串口3寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //准备设置PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //准备设置PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口3的接收
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
      
    USART_Init(USART3, &USART_InitStructure);                                      //设置串口3	

	USART_ClearFlag(USART3, USART_FLAG_RXNE);	              //清除接收标志位
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //设置串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                          //设置串口3中断 

//	USART_Cmd(USART3, ENABLE);                                //使能串口3
}

/*
 * 函数名：GPS_USART_Config   使用PB10 PB11 即是串口3与GPS通信
 * 描述  ：USART GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
static void GPS_USART_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  
	/* config USART clock */
  // 使能GPIO时钟
//	RCC_AHBPeriphClockCmd(GPS_DMA_CLK, ENABLE);
	// 使能串口时钟
	
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
	USART_InitStructure.USART_BaudRate = GPS_USART_BAUDRATE;                //GPS模块默认使用波特率：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(GPS_USART, &USART_InitStructure); 
	// 中断设置
  GPS_Interrupt_Config();
//	USART_Cmd(GPS_USART, ENABLE);	
	u1_printf("hello4");
	
}

/**
  * @brief  GPS_Config gps 初始化
  * @param  无
  * @retval 无
  */
void GPS_Config(void)
{
//  GPS_USART_Config();
	Usart3_Init(9600);
//  GPS_DMA_Config(); 
//  decode_init();
}


/*********************************************有用的设置,其他都是废的**********************************************/

/**
  * @brief  GPS_DMA_Config gps dma接收配置
  * @param  无
  * @retval 无
  */
static void GPS_DMA_Config(void)
{
   DMA_InitTypeDef DMA_InitStructure;
	 u1_printf("hello5");
	
		/*开启DMA时钟*/
		RCC_AHBPeriphClockCmd(GPS_DMA_CLK, ENABLE);

		/*设置DMA源：串口数据寄存器地址*/
		DMA_InitStructure.DMA_PeripheralBaseAddr = GPS_DATA_ADDR;	   

		/*内存地址(要传输的变量的指针)*/
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)gps_rbuff;

		/*方向：从内存到外设*/		
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	

		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = GPS_RBUFF_SIZE;

		/*外设地址不增*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*内存地址自增*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*外设数据单位*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*内存数据单位 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMA模式：不断循环*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*优先级：中*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  

		/*禁止内存到内存的传输	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*配置DMA的通道*/		   
		DMA_Init(GPS_DMA_CHANNEL, &DMA_InitStructure); 	   
    
//    GPS_Interrupt_Config();
		
    DMA_ITConfig(GPS_DMA_CHANNEL,DMA_IT_HT|DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断

		/*使能DMA*/
//		DMA_Cmd (GPS_DMA_CHANNEL,ENABLE);		
    
    /* 配置串口 向 DMA发出TX请求 */
//		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, ENABLE);
		u1_printf("hello6");
}



nmeaINFO info;          //GPS解码后得到的信息
nmeaPARSER parser;      //解码时使用的数据结构  
uint8_t new_parse;    //是否有新的解码数据标志

nmeaTIME beiJingTime;    //北京时间 

// 用来初始化gps解码数据解构 防止多次生成销毁
void decode_init(void) {
//	  nmeaINFO info;          //GPS解码后得到的信息
//    nmeaPARSER parser;      //解码时使用的数据结构  
//    uint8_t new_parse=0;    //是否有新的解码数据标志
//  
//    nmeaTIME beiJingTime;    //北京时间 
	  new_parse = 0;

    /* 设置用于输出调试信息的函数 */
    nmea_property()->trace_func = &trace;
    nmea_property()->error_func = &error;
    nmea_property()->info_func = &gps_info;

    /* 初始化GPS数据结构 */
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
}

/**
  * @brief  GPS_ProcessDMAIRQ GPS DMA中断服务函数
  * @param  None.
  * @retval None.
  */
void GPS_ProcessDMAIRQ(void)
{  
	u1_printf("hello9999");
  if(DMA_GetITStatus(GPS_DMA_IT_HT) )         /* DMA 半传输完成 */
  {
    GPS_HalfTransferEnd = 1;                //设置半传输完成标志位
    DMA_ClearFlag(GPS_DMA_FLAG_HT);
  }
  else if(DMA_GetITStatus(GPS_DMA_IT_TC))     /* DMA 传输完成 */
  {
    GPS_TransferEnd = 1;                    //设置传输完成标志位
    DMA_ClearFlag(GPS_DMA_FLAG_TC);

   }
}



/**
  * @brief  trace 在解码时输出捕获的GPS语句
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void trace(const char *str, int str_size)
{
  #ifdef __GPS_DEBUG    //在gps_config.h文件配置这个宏，是否输出调试信息
    uint16_t i;
    u1_printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      u1_printf("%c",*(str+i));
  
    u1_printf("\n");
  #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
  */
void error(const char *str, int str_size)
{
    #ifdef __GPS_DEBUG   //在gps_config.h文件配置这个宏，是否输出调试信息

    uint16_t i;
    u1_printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      u1_printf("%c",*(str+i));
    u1_printf("\n");
    #endif
}

/**
  * @brief  error 在解码出错时输出提示消息
  * @param  str: 要输出的字符串，str_size:数据长度
  * @retval 无
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
**     函数名称:            bit        IsLeapYear(uint8_t    iYear) 
**    功能描述:            判断闰年(仅针对于2000以后的年份) 
**    入口参数：            iYear    两位年数 
**    出口参数:            uint8_t        1:为闰年    0:为平年 
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
**     函数名称:            void    GMTconvert(uint8_t *DT,uint8_t GMT,uint8_t AREA) 
**    功能描述:            格林尼治时间换算世界各时区时间 
**    入口参数：            *DT:    表示日期时间的数组 格式 YY,MM,DD,HH,MM,SS 
**                        GMT:    时区数 
**                        AREA:    1(+)东区 W0(-)西区 
********************************************************************************************************/ 
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //年月日时分秒暂存变量 
     
    if(GMT==0)    return;                //如果处于0时区直接返回 
    if(GMT>12)    return;                //时区最大为12 超过则返回         

    YY    =    SourceTime->year;                //获取年 
    MM    =    SourceTime->mon;                 //获取月 
    DD    =    SourceTime->day;                 //获取日 
    hh    =    SourceTime->hour;                //获取时 
    mm    =    SourceTime->min;                 //获取分 
    ss    =    SourceTime->sec;                 //获取秒 

    if(AREA)                        //东(+)时区处理 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//如果与格林尼治时间处于同一天则仅加小时即可 
        else                        //如果已经晚于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+GMT-24;        //先得出时间 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //大月份(12月单独处理) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //小月份2月单独处理) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //处理2月份 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //本来是闰年且是2月29日 或者不是闰年且是2月28日 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //处理12月份 
            { 
                if(DD<31)    DD++; 
                else        //跨年最后一天 
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
        if(hh>=GMT)    hh    -=    GMT;    //如果与格林尼治时间处于同一天则仅减小时即可 
        else                        //如果已经早于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+24-GMT;        //先得出时间 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //上月是大月份(1月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //上月是小月份2月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //处理上个月是2月份 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //不是闰年 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //处理1月份 
            { 
                if(DD>1)    DD--; 
                else        //新年第一天 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //更新年 
    ConvertTime->mon    =    MM;                //更新月 
    ConvertTime->day    =    DD;                //更新日 
    ConvertTime->hour   =    hh;                //更新时 
    ConvertTime->min    =    mm;                //更新分 
    ConvertTime->sec    =    ss;                //更新秒 
}  






/*********************************************************end of file**************************************************/
