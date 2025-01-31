#ifndef __BEEPER_H
#define __BEEPER_H

#include "stm32f10x.h"                  // Device header

#define BEEPER_INIT(port, pin) Gpio_Init(RCC_APB2Periph_##port, port, pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz)
#define BEEPER_ON(port, pin)   GPIO_OFF(port, pin)
#define BEEPER_OFF(port, pin)  GPIO_ON(port, pin)
void Beeper_Test(void);


#endif
