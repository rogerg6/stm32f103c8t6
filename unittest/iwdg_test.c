#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "key.h"

void IWDG_Init(void)
{
	// 超时时间=1000ms
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	// 对presc和reload寄存器解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_16);			
	IWDG_SetReload(2499);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void IWDG_Test(void)
{
	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_0);		// PB0接一个按键，一端接地
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET) {
		OLED_ShowString(2, 1, "IWDG_RST");
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
	
	IWDG_Init();

	
	while (1){
		Is_KeyDown(GPIOB, GPIO_Pin_0);		// 阻塞获取
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");				//OLED闪烁FEED字符串
		Delay_ms(200);								//喂狗间隔为200+600=800ms
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);
	}
}