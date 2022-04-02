#ifndef	__SHT30_H
#define __SHT30_H

#include "mytype.h"


extern float Temperature_sht;
extern float Humidity_sht;


extern u8 Refresh_SHT30_Data;
extern u8 send_data_fleg;
extern uint8_t humiture_buff1[100];


void SHT30_Init(void);
void SHT30_read_result(u8 addr);

#endif
