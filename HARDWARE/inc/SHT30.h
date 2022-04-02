#ifndef	__SHT30_H
#define __SHT30_H

#include "mytype.h"

extern float Temperature_sht;
extern float Humidity_sht;

extern u8 humiture_buff1[20];
extern u8 humiture_buff2[20];
extern u8 Refresh_SHT30_Data;
extern u8 send_data_fleg;
extern u8 Temperature_L;
extern u8 Humidity_L;
extern u8 Temperature_H;
extern u8 Humidity_H;

void SHT30_Init(void);
void SHT30_read_result(u8 addr);

#endif
