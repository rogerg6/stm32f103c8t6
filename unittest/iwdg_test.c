/**
 * @brief 独立看门狗有1个参数：超时时间cnt
 *		  假设cnt=1000ms, 那么从喂狗开始算起，在1000ms以内喂狗能成功；
 *		  在1000ms之外喂狗，就会触发看门狗复位
 *
 * 硬件连接：
 *		OLED屏
 *		PB0接一个按键，一端接地。用来产生超时喂狗
**/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "key.h"

void IWDG_Init(void)
{
	// 超时时间=1000ms
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	// 对presc和reload寄存器解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_16);			// 设置预分频值	
	IWDG_SetReload(2499);							// 设置cnt值
	IWDG_ReloadCounter();							// 喂狗
	IWDG_Enable();									// 使能看门狗
}

void IWDG_Test(void)
{
	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_0);		
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	// 判断上一次复位类型
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