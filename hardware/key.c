#include "Delay.h"
#include "gpio.h"

uint8_t Is_KeyDown(GPIO_TypeDef* port, uint16_t pin)
{
	if (GPIO_ReadInputDataBit(port, pin) == Bit_RESET) {
		Delay_ms(20);	// 防抖动
		while (GPIO_ReadInputDataBit(port, pin) == Bit_RESET);
		Delay_ms(20);	// 防抖动
		return 1;
	} else
		return 0;
}



