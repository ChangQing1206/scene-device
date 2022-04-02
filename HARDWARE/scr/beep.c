#include "beep.h"

void BEEP_Init()
{
	BEEP_OFF;
}

void BEEP_Last(int delay_time)
{
	BEEP_ON;
	DelayMs(delay_time);
	BEEP_OFF;
}
