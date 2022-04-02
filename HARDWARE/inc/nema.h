#ifndef __NEMA_H
#define __NEMA_H

#include "stm32f10x.h"
#include "gps_config.h"
#include "nmea/nmea.h"


extern double longitude;
extern double latitude;
void nema_decode_test(void);


#endif


