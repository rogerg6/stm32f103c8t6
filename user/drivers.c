#include "drivers.h"
#include "stm32f10x.h"                  // Device header


static void Delay(__IO uint32_t t)
{
	while(t--);
}

void Led_Test(void)
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
		Delay(0xfffff);
		// 关闭led
		GPIO_SetBits(GPIOC, GPIO_Pin_13);	
		Delay(0xfffff);
	}
	
}

