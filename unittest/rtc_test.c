#include "rtc.h"
#include <time.h>
#include "OLED.h"
#include "Delay.h"


void RTC_Test(void)
{
	
	struct RTC_Date date;

	OLED_Init();
	RTCInit();
	
	OLED_ShowString(1, 1, "DATE: xxxx-xx-xx");
	OLED_ShowString(2, 1, "TIME: xx:xx:xx");
	OLED_ShowString(3, 1, "CNT : ");
	OLED_ShowString(4, 1, "DIV : ");
	
	while (1) {
		RTC_GetTime(&date);
		OLED_ShowNum(1, 7,  date.year, 4);
		OLED_ShowNum(1, 12, date.month, 2);
		OLED_ShowNum(1, 15, date.day, 2);
		OLED_ShowNum(2, 7,  date.hour, 2);
		OLED_ShowNum(2, 10, date.min, 2);
		OLED_ShowNum(2, 13, date.sec, 2);
		OLED_ShowNum(3, 7, RTC_GetCounter(), 10);								// timestamp
		OLED_ShowNum(4, 7, (32767 - RTC_GetDivider()) * 999 / 32767, 10);		// ms
	}
}

/** 
 * standby模式下rtc测试
 * 
 * 1. 每个10s OLED上显示一下数据
 * 2. 手动拉高PA0，即WKUP引脚，会唤醒系统
**/
void RTC_StandbyMode_Test(void)
{
	uint32_t alarm_val;
	
	OLED_Init();
	RTCInit();
	
	//开启PWR的时钟
	//停止模式和待机模式一定要记得开启
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		
	
	
	OLED_ShowString(1, 1, "CNT:");		// timestamp
	OLED_ShowString(2, 1, "ALR:");		// alarm timestamp = timestamp + 10
	OLED_ShowString(3, 1, "ALRM:");		// alarm标志位

	/*使能WKUP引脚*/
	PWR_WakeUpPinCmd(ENABLE);
	
	alarm_val = RTC_GetCounter() + 10;	// 10s后启动alarm中断
	RTC_SetAlarm(alarm_val);
	OLED_ShowNum(2, 5, alarm_val, 10);	

	while (1) {
		OLED_ShowNum(1, 5, RTC_GetCounter(), 10);								// 显示时间戳
		OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);			    	// 显示alarm标志位

		OLED_ShowString(4, 1, "Running");
		Delay_ms(1000);
		OLED_ShowString(4, 1, "Standby");
		Delay_ms(100);
		
		/* TODO: 把所有的外设都关掉或者降低功耗 */
		OLED_Clear();
		
		/***
		 * 进入standby模式
		**/
		PWR_EnterSTANDBYMode();
		
		// wakeup后，程序会重新开始，和上电一样
	}
}


