#include "gpio.h"

void Gpio_Init(uint32_t clk, GPIO_TypeDef* port, uint16_t pins, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef gpio_cfg;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(clk, ENABLE);
	
	gpio_cfg.GPIO_Mode = mode;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = pins;
	gpio_cfg.GPIO_Speed = speed;		// gpio切换频率
	GPIO_Init(port, &gpio_cfg);
}


/***
 * @brief GPIO输出引脚翻转状态
 */
void Gpio_Pin_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	if (GPIO_ReadOutputDataBit(GPIOx, pin) == 0)
		GPIO_SetBits(GPIOx, pin);
	else
		GPIO_ResetBits(GPIOx, pin);
}

