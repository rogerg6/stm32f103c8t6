/***
 * @brief rtc实时时钟，设置/显示时间
 *
 * 硬件连接：stlink的3.3v连接VBAT引脚，给备份寄存器单独供电
 */

#include "stm32f10x.h"                  // Device header
#include <time.h>
#include "OLED.h"
#include "rtc.h"

void RTC_SetTime(void);

struct RTC_Date rtc_date = {
	.year = 2023,
	.month = 1,
	.day = 1,
	.hour = 23,
	.min = 59,
	.sec = 57,
};

void RTCInit(void)
{
	// 使能BKP
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	/* 通过往BKP中写入标志位数据，来判断系统是否重新上下电。
	 * 如果vbat下电走if分支，否则走else分支
	 */
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A) {
		// 使能LSE
		RCC_LSEConfig(RCC_LSE_ON);
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	// 等待LSE ready
		
		// 选择时钟源
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		// 使能rtc
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();		// 等待同步
		RTC_WaitForLastTask();		// 等待上一个操作完成
		
		// 配置预分频器
		RTC_SetPrescaler(32768 - 1);
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	
		RTC_SetTime();
		BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
	} else {
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
	
}

void RTC_SetTime(void)
{
	struct tm tmd;
	time_t ts;
	
	tmd.tm_year = rtc_date.year - 1900;
	tmd.tm_mon  = rtc_date.month - 1;
	tmd.tm_mday = rtc_date.day;
	tmd.tm_hour = rtc_date.hour;
	tmd.tm_min  = rtc_date.min;
	tmd.tm_sec  = rtc_date.sec;
	
	ts = mktime(&tmd) - 8 * 60 * 60;		// 当前时区是东八区，比GMT时间早了8h
	RTC_SetCounter(ts);
	RTC_WaitForLastTask();
}

void RTC_GetTime(void)
{
	struct tm tmd;
	time_t ts;

	ts = RTC_GetCounter() + 8 * 60 *60;
	tmd = *localtime(&ts);
	rtc_date.year  = tmd.tm_year + 1900;
    rtc_date.month = tmd.tm_mon + 1;
    rtc_date.day   = tmd.tm_mday;
    rtc_date.hour  = tmd.tm_hour;
    rtc_date.min   = tmd.tm_min;
    rtc_date.sec   = tmd.tm_sec;
}

void RTC_Test(void)
{
	OLED_Init();
	RTCInit();
	
	OLED_ShowString(1, 1, "DATE: xxxx-xx-xx");
	OLED_ShowString(2, 1, "TIME: xx:xx:xx");
	OLED_ShowString(3, 1, "CNT : ");
	OLED_ShowString(4, 1, "DIV : ");
	
	while (1) {
		RTC_GetTime();
		OLED_ShowNum(1, 7, rtc_date.year, 4);
		OLED_ShowNum(1, 12, rtc_date.month, 2);
		OLED_ShowNum(1, 15, rtc_date.day, 2);
		OLED_ShowNum(2, 7, rtc_date.hour, 2);
		OLED_ShowNum(2, 10, rtc_date.min, 2);
		OLED_ShowNum(2, 13, rtc_date.sec, 2);
		OLED_ShowNum(3, 7, RTC_GetCounter(), 10);								// timestamp
		OLED_ShowNum(4, 7, (32767 - RTC_GetDivider()) * 999 / 32767, 10);		// ms
	}
}

