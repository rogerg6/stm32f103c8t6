#ifndef __RTC_H
#define __RTC_H

struct RTC_Date {
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  min;
	uint8_t  sec;
};

void RTC_Test(void);

#endif
