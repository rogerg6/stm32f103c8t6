#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"


/**
 * @brief 降低主频能降低系统功耗。
 *        修改system_stm32f10x.c Line109 else中定义的宏即可达到修改主频的目的
 */
void Change_Main_Freq(void)
{
	OLED_Init();
	
	OLED_ShowNum(1, 1, SystemCoreClock, 8);

	while (1) {
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
}

