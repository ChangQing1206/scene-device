/**
  ******************************************************************************
  * @file    nmea_decode_test.c
  * @author  fire
  * @version V1.0
  * @date    2016-07-xx
  * @brief   ����NEMA�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��F103������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
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

γ�ȣ�22.597196,����113.080710

���θ߶ȣ�67.600000 �� 
�ٶȣ�0.000000 km/h 
����160.730000 ��
����ʹ�õ�GPS���ǣ�6,�ɼ�GPS���ǣ�13
����ʹ�õı������ǣ�3,�ɼ��������ǣ�3
PDOP��2.400000,HDOP��1.600000��VDOP��1.800000
ʱ��2022-04-01,21:14:43

*/
  
    //��GPSģ�鴫�ص���Ϣ���н���

/**
  * @brief  nmea_decode_test ����GPSģ����Ϣ
  * @param  ��
  * @retval ��
  */
void nema_decode_test(void)
{	
		double deg_lat = 0;//ת����[degree].[degree]��ʽ��γ��
		double deg_lon = 0;//ת����[degree].[degree]��ʽ�ľ���
	  GPS_HalfTransferEnd = 0;
		GPS_TransferEnd = 0;
		u1_printf("decode1");
		  		/*ʹ��DMA*/
		DMA_Cmd (GPS_DMA_CHANNEL,ENABLE);		
    
    /* ���ô��� �� DMA����TX���� */
		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, ENABLE);
		// ����γ��Ϊ0ʱ���� ��γ�Ȳ�Ϊ0ʱ��ѭ��
    while(!deg_lat && !deg_lon)
    {
			u1_printf("decode2");
      if(GPS_HalfTransferEnd)     /* ���յ�GPS_RBUFF_SIZEһ������� */
      {
        /* ����nmea��ʽ���� */
        nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
        u1_printf("decode3");
        GPS_HalfTransferEnd = 0;   //��ձ�־λ
        new_parse = 1;             //���ý�����Ϣ��־ 
      }
      else if(GPS_TransferEnd)    /* ���յ���һ������ */
      {
				u1_printf("decode4");
        nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
        
        GPS_TransferEnd = 0;
        new_parse =1;
      }
      u1_printf("decode5");
      if(new_parse )                //���µĽ�����Ϣ   
      {    
        /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
        GMTconvert(&info.utc,&beiJingTime,8,1);
        
        /* �������õ�����Ϣ */
				u1_printf("\r\nʱ��%d-%02d-%02d,%d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
				
				//info.lat lon�еĸ�ʽΪ[degree][min].[sec/60]��ʹ�����º���ת����[degree].[degree]��ʽ
				deg_lat = nmea_ndeg2degree(info.lat);
				deg_lon = nmea_ndeg2degree(info.lon);
				// ��ֵ
				longtitude = deg_lon;
				latitude = deg_lat;
				u1_printf("decode6");
				// ���Դ�ӡʹ��
				u1_printf("\r\nγ�ȣ�%f,����%f\r\n",deg_lat,deg_lon);
        u1_printf("\r\n���θ߶ȣ�%f �� ", info.elv);
        u1_printf("\r\n�ٶȣ�%f km/h ", info.speed);
        u1_printf("\r\n����%f ��", info.direction);
				
				u1_printf("\r\n����ʹ�õ�GPS���ǣ�%d,�ɼ�GPS���ǣ�%d",info.satinfo.inuse,info.satinfo.inview);

				u1_printf("\r\n����ʹ�õı������ǣ�%d,�ɼ��������ǣ�%d",info.BDsatinfo.inuse,info.BDsatinfo.inview);
				u1_printf("\r\nPDOP��%f,HDOP��%f��VDOP��%f",info.PDOP,info.HDOP,info.VDOP);
        
        new_parse = 0;
      }
	
	}
				/*�ر�DMA*/
		DMA_Cmd (GPS_DMA_CHANNEL,DISABLE);		
    
    /* ���ô��� �� DMA����TX���� */
		USART_DMACmd(GPS_USART, USART_DMAReq_Rx, DISABLE);
		

    /* �ͷ�GPS���ݽṹ */
    // nmea_parser_destroy(&parser);

    
    //  return 0;
		
//		longitude = 113.046780;
//		latitude = 23.092880;
}








/**************************************************end of file****************************************/

