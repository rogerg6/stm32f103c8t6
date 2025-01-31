#include "led.h"
#include "Delay.h"

/***
 * @brief 核心板自带的led闪烁. 
 */
void Led_Blink(void)
{
	LED_INIT(GPIOC, GPIO_Pin_13);
	
	while (1)
	{
		// 点亮led
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);	
		Delay_ms(500);
		// 关闭led
		GPIO_SetBits(GPIOC, GPIO_Pin_13);	
		Delay_ms(500);
	}
	
}

/***
 * @brief led流水灯. 8个led负极分别接到PA0~7, 正极接VCC
 * @parma ms延迟时间
 */
void Led_TubeLight(uint32_t ms)
{
	int i;
	uint16_t val;
		
	LED_INIT(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |\
						GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
						GPIO_Pin_6 | GPIO_Pin_7);

	
	// 全亮全灭
	GPIO_Write(GPIOA, 0x0);
	Delay_ms(10 * ms);
	GPIO_Write(GPIOA, 0xffff);
	
	while (1) {
		val = 1;
		for (i = 0; i < 8; i++) {
			GPIO_WriteBit(GPIOA, val, Bit_RESET);	
			Delay_ms(ms);
			GPIO_WriteBit(GPIOA, val, Bit_SET);	
			Delay_ms(ms);
			val <<= 1;
		}
	}
	
}
