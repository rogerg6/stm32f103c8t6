#ifndef __PHOTORESISTER_H
#define __PHOTORESISTER_H

#include "stm32f10x.h"                  // Device header
#include "gpio.h"

#define PHOTORESISTER_INIT(port, pins) \
			Gpio_Init(RCC_APB2Periph_##port, port, pins, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz)

void Photoresistor_Beeper(void);


#endif
