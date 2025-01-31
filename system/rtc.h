#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"                  // Device header


struct RTC_Date {
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  min;
	uint8_t  sec;
};

void RTCInit(void);
void RTC_SetTime(struct RTC_Date *rtc_date);
void RTC_GetTime(struct RTC_Date *date);

#endif
