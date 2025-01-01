#include "gpio.h"

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