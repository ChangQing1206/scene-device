/**
  ******************************************************************************
  * @file    nmea_decode_test.c
  * @author  fire
  * @version V1.0
  * @date    2016-07-xx
  * @brief   测试NEMA解码库
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火F103开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "nema.h"
#include "usart1.h"
//#include "stm32f10x.h"
////#include "./usart/bsp_usart.h"
#include "gps_config.h"
#include "nmea/nmea.h"

/*

纬度：22.597196,经度113.080710

海拔高度：67.600000 米 
速度：0.000000 km/h 
航向：160.730000 度
正在使用的GPS卫星：6,可见GPS卫星：13
正在使用的北斗卫星：3,可见北斗卫星：3
PDOP：2.400000,HDOP：1.600000，VDOP：1.800000
时间2022-04-01,21:14:43

*/
  
    //对GPS模块传回的信息进行解码

/**
  * @brief  nmea_decode_test 解码GPS模块信息
  * @param  无
  * @retval 无
  */
void nema_decode_test(void)
{	
		double deg_lat = 0;//转换成[degree].[degree]格式的纬度
		double deg_lon = 0;//转换成[degree].[degree]格式的经度
	  GPS_HalfTransferEnd = 0;
		GPS_TransferEnd = 0;
		u1_printf("decode1");
		  		/*使能DMA*/
		DMA_Cmd (GPS_DMA_CHANNEL,ENABLE);		
    
    /* 配置串口 向 DMA发出TX请求 */
		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, ENABLE);
		// 当经纬度为0时进入 经纬度不为0时出循环
    while(!deg_lat && !deg_lon)
    {
			u1_printf("decode2");
      if(GPS_HalfTransferEnd)     /* 接收到GPS_RBUFF_SIZE一半的数据 */
      {
        /* 进行nmea格式解码 */
        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
        u1_printf("decode3");
        GPS_HalfTransferEnd = 0;   //清空标志位
        new_parse = 1;             //设置解码消息标志 
      }
      else if(GPS_TransferEnd)    /* 接收到另一半数据 */
      {
				u1_printf("decode4");
        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
        
        GPS_TransferEnd = 0;
        new_parse =1;
      }
      u1_printf("decode5");
      if(new_parse )                //有新的解码消息   
      {    
        /* 对解码后的时间进行转换，转换成北京时间 */
        GMTconvert(&info.utc,&beiJingTime,8,1);
        
        /* 输出解码得到的信息 */
				u1_printf("\r\n时间%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
				
				//info.lat lon中的格式为[degree][min].[sec/60]，使用以下函数转换成[degree].[degree]格式
				deg_lat = nmea_ndeg2degree(info.lat);
				deg_lon = nmea_ndeg2degree(info.lon);
				// 赋值
				longtitude = deg_lon;
				latitude = deg_lat;
				u1_printf("decode6");
				// 调试打印使用
				u1_printf("\r\n纬度：%f,经度%f\r\n",deg_lat,deg_lon);
        u1_printf("\r\n海拔高度：%f 米 ", info.elv);
        u1_printf("\r\n速度：%f km/h ", info.speed);
        u1_printf("\r\n航向：%f 度", info.direction);
				
				u1_printf("\r\n正在使用的GPS卫星：%d,可见GPS卫星：%d",info.satinfo.inuse,info.satinfo.inview);

				u1_printf("\r\n正在使用的北斗卫星：%d,可见北斗卫星：%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
				u1_printf("\r\nPDOP：%f,HDOP：%f，VDOP：%f",info.PDOP,info.HDOP,info.VDOP);
        
        new_parse = 0;
      }
	
	}
				/*关闭DMA*/
		DMA_Cmd (GPS_DMA_CHANNEL,DISABLE);		
    
    /* 配置串口 向 DMA发出TX请求 */
		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, DISABLE);
		

    /* 释放GPS数据结构 */
    // nmea_parser_destroy(&parser);

    
    //  return 0;
		
//		longitude = 113.046780;
//		latitude = 23.092880;
}








/**************************************************end of file****************************************/

