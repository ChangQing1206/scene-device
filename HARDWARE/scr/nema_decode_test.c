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
//#include "gps_config.h"
//#include "nmea/nmea.h"

  
    //对GPS模块传回的信息进行解码

/**
  * @brief  nmea_decode_test 解码GPS模块信息
  * @param  无
  * @retval 无
  */
void nema_decode_test(void)
{
//		double deg_lat;//转换成[degree].[degree]格式的纬度
//		double deg_lon;//转换成[degree].[degree]格式的经度
//	
//    nmeaINFO info;          //GPS解码后得到的信息
//    nmeaPARSER parser;      //解码时使用的数据结构  
//    uint8_t new_parse=0;    //是否有新的解码数据标志
//  
//    nmeaTIME beiJingTime;    //北京时间 

//    /* 设置用于输出调试信息的函数 */
//    nmea_property()->trace_func = &trace;
//    nmea_property()->error_func = &error;
//    nmea_property()->info_func = &gps_info;

//    /* 初始化GPS数据结构 */
//    nmea_zero_INFO(&info);
//    nmea_parser_init(&parser);

//    while(1)
//    {
//      if(GPS_HalfTransferEnd)     /* 接收到GPS_RBUFF_SIZE一半的数据 */
//      {
//        /* 进行nmea格式解码 */
//        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
//        
//        GPS_HalfTransferEnd = 0;   //清空标志位
//        new_parse = 1;             //设置解码消息标志 
//      }
//      else if(GPS_TransferEnd)    /* 接收到另一半数据 */
//      {

//        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
//       
//        GPS_TransferEnd = 0;
//        new_parse =1;
//      }
//      
//      if(new_parse )                //有新的解码消息   
//      {    
//        /* 对解码后的时间进行转换，转换成北京时间 */
//        GMTconvert(&info.utc,&beiJingTime,8,1);
//        
//        /* 输出解码得到的信息 */
//				u1_printf("\r\n时间%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
//				
//				//info.lat lon中的格式为[degree][min].[sec/60]，使用以下函数转换成[degree].[degree]格式
//				deg_lat = nmea_ndeg2degree(info.lat);
//				deg_lon = nmea_ndeg2degree(info.lon);
//				
//				u1_printf("\r\n纬度：%f,经度%f\r\n",deg_lat,deg_lon);
//        u1_printf("\r\n海拔高度：%f 米 ", info.elv);
//        u1_printf("\r\n速度：%f km/h ", info.speed);
//        u1_printf("\r\n航向：%f 度", info.direction);
//				
//				u1_printf("\r\n正在使用的GPS卫星：%d,可见GPS卫星：%d",info.satinfo.inuse,info.satinfo.inview);

//				u1_printf("\r\n正在使用的北斗卫星：%d,可见北斗卫星：%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
//				u1_printf("\r\nPDOP：%f,HDOP：%f，VDOP：%f",info.PDOP,info.HDOP,info.VDOP);
//        
//        new_parse = 0;
//      }
	
	}

    /* 释放GPS数据结构 */
    // nmea_parser_destroy(&parser);

    
    //  return 0;
}








/**************************************************end of file****************************************/

