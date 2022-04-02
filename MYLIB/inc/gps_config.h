#ifndef __GPS_CONFIG_H
#define	__GPS_CONFIG_H

#include "stm32f10x.h"
#include "nmea/nmea.h"
#include "string.h"


/* 配置 */

//#define __GPS_DEBUG     //定义这个宏，在nmea解码时会输出调试信息

//#define __GPS_LOG_FILE    //定义这个宏，对SD卡上的gpslog.txt文件进行解码；不定义的话使用串口接收GPS信息解码


/* GPS接口配置 使用不同的串口时，要修改对应的接口 */

#define GPS_DR_Base            (USART3_BASE+0x04)		  // 串口的数据寄存器地址


#define GPS_DATA_ADDR             GPS_DR_Base        //GPS使用的串口的数据寄存器地址
#define GPS_RBUFF_SIZE            512                   //串口接收缓冲区大小
#define HALF_GPS_RBUFF_SIZE       (GPS_RBUFF_SIZE/2)    //串口接收缓冲区一半  

/* 外设 */
/*******************************************************/
//GPS串口波特率
#define GPS_USART_BAUDRATE                    9600

#define GPS_USART                             USART3
#define GPS_USART_CLK                         RCC_APB1Periph_USART3
#define GPS_RCC_PeriphClockCmd_Fun            RCC_APB1PeriphClockCmd

#define GPS_USART_RX_GPIO_PORT                GPIOB
#define GPS_USART_RX_GPIO_CLK                 RCC_APB2Periph_GPIOB
#define GPS_USART_RX_PIN                      GPIO_Pin_11


#define GPS_USART_TX_GPIO_PORT                GPIOB
#define GPS_USART_TX_GPIO_CLK                 RCC_APB2Periph_GPIOB
#define GPS_USART_TX_PIN                      GPIO_Pin_10
/************************************************************/
#define GPS_DMA                      DMA1
#define GPS_DMA_CLK                  RCC_AHBPeriph_DMA1
#define GPS_DMA_CHANNEL              DMA1_Channel3
#define GPS_DMA_IRQn                 DMA1_Channel3_IRQn         //GPS中断源

/* 外设标志 */
#define GPS_DMA_FLAG_TC              DMA1_FLAG_TC3
#define GPS_DMA_FLAG_TE              DMA1_FLAG_TE3
#define GPS_DMA_FLAG_HT              DMA1_FLAG_HT3
#define GPS_DMA_FLAG_GL              DMA1_FLAG_GL3
#define GPS_DMA_IT_HT                DMA1_IT_HT3
#define GPS_DMA_IT_TC                DMA1_IT_TC3


/* 中断函数 */
//#define GPS_DMA_IRQHANDLER           DMA1_Channel3_IRQHandler   //GPS使用的DMA中断服务函数


extern uint8_t gps_rbuff[GPS_RBUFF_SIZE];
extern __IO uint8_t GPS_TransferEnd ;
extern __IO uint8_t GPS_HalfTransferEnd;


// gps数据解码
extern nmeaINFO info;          //GPS解码后得到的信息
extern nmeaPARSER parser;      //解码时使用的数据结构  
extern uint8_t new_parse;    //是否有新的解码数据标志

extern nmeaTIME beiJingTime;    //北京时间 


/*********************************************有用的设置,其他都是废的**********************************************/
//定义经纬度变量  全局变量
extern double longtitude;
extern double latitude;
extern char temp_gpsdata[73];
extern char i;
extern char USART_RX_OK_FLAG;
extern char gps_data[73];
int num_pow(int num1,int num2);
float NMAE_str2num(char buf[]);
unsigned char NMAE_find_pos(unsigned char num);
void GPS_data_process();
//获取自身的GPS信息
void GET_GPS(void);
/*********************************************有用的设置,其他都是废的**********************************************/
void GPS_ProcessDMAIRQ(void);
void GPS_Config(void);

void trace(const char *str, int str_size);
void error(const char *str, int str_size);
void gps_info(const char *str, int str_size);
void GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) ;
void decode_init(void);



#endif 
