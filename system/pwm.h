#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"                  // Device header

void PWM_Init(uint32_t freq, uint16_t cycle, uint16_t duty);
void PWM_SetDuty(float dr);

#endif


