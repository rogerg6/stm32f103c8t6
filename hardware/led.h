

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header
#include "gpio.h"


#define LED_INIT(port, pins) Gpio_Init(RCC_APB2Periph_##port, port, pins, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)

void Led_Blink(void);
void Led_TubeLight(uint32_t ms);





#endif