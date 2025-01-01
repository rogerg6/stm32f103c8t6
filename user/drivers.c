#include "drivers.h"

// led闪烁
void Led_Blink(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_13;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOC, &gpio_cfg);
	
	
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

// led流水灯
void Led_TubeLight(uint32_t ms)
{
	GPIO_InitTypeDef gpio_cfg;
	int i;
	uint16_t val;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |\
						GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
						GPIO_Pin_6 | GPIO_Pin_7;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOA, &gpio_cfg);
	
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


