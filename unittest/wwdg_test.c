/**
 * @brief 窗口看门狗有2个参数：超时时间cnt，窗口时间win
 *		  假设cnt=50ms, win=30ms, 那么从喂狗开始算起，在50ms
 *		  [0, 30]或[50, ∞)喂狗，都会触发看门狗复位；只有在50ms的
 *		  (30, 50)喂狗才能成功
 *
 * 硬件连接：
 *		OLED屏
 *		PB0接一个按键，一端接地。用来产生超时喂狗
**/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "key.h"

// 设置超时时间50ms，窗口值30ms
void WWDG_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);		//wwdg使用pclk1时钟源
	WWDG_SetWindowValue(0x40 | 21);							// 设置窗口值	
	WWDG_SetPrescaler(WWDG_Prescaler_8);				// 设置预分频值	
	WWDG_Enable(0x40 | 54);								// 使能wwdg同时喂狗
}

void WWDG_Test(void)
{
	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_0);
	
	OLED_ShowString(1, 1, "WWDG TEST");
	
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) {
		OLED_ShowString(2, 1, "WWDG_RST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "        ");
		Delay_ms(200);
		RCC_ClearFlag();
	} else {
		OLED_ShowString(3, 1, "PIN_RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "       ");
		Delay_ms(200);
		RCC_ClearFlag();
	}
	
	WWDG_Init();

	while (1){
		Is_KeyDown(GPIOB, GPIO_Pin_0);		// 阻塞获取
			
		OLED_ShowString(4, 1, "FEED");				//OLED闪烁FEED字符串
		Delay_ms(20);								//喂狗间隔为40ms
		OLED_ShowString(4, 1, "    ");
		Delay_ms(20);
		
		WWDG_SetCounter(0x40 | 54);			// 喂狗

	}
}