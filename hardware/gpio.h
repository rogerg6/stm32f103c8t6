#ifndef __MY_GPIO_H
#define __MY_GPIO_H

#include "stm32f10x.h"                  // Device header


void Gpio_Init(uint32_t clk, GPIO_TypeDef* port, uint16_t pins, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
void Gpio_Pin_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin);


//#define GPIO_INIT(port, pins, mode, speed) Gpio_Init(RCC_APB2Periph_##port, port, pins, mode, speed)
#define GPIO_INIT(port, pins, mode, speed) \
	do {                                                               \
		GPIO_InitTypeDef gpio_cfg;                                     \
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_##port, ENABLE);         \
																	   \
		gpio_cfg.GPIO_Mode = mode;	                                   \
		gpio_cfg.GPIO_Pin = pins;                                      \
		gpio_cfg.GPIO_Speed = speed;                                   \
		GPIO_Init(port, &gpio_cfg);                                    \
	}while (0)

#define GPIO_ON(port, pins)  GPIO_WriteBit(port, pins, (BitAction)1)
#define GPIO_OFF(port, pins) GPIO_WriteBit(port, pins, (BitAction)0)
#define GPIO_TOGGLE(port, pins) Gpio_Pin_Toggle(port, pins)


#endif


