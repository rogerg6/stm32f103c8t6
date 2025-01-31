#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"                  // Device header
#include "gpio.h"

#define KEY_INIT(port, pins) Gpio_Init(RCC_APB2Periph_##port, port, pins, GPIO_Mode_IPU, GPIO_Speed_50MHz)
uint8_t Is_KeyDown(GPIO_TypeDef* port, uint16_t pin);



#endif
