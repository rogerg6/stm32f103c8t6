#ifndef __IC_H
#define __IC_H

#include "stm32f10x.h"                  // Device header

void IC3_1_Init(uint32_t freq, uint8_t is_pwm);
#define IC3_1_GetCount()	  (TIM_GetCapture1(TIM3) + 1)
#define IC3_1_GetDutyCount()  (TIM_GetCapture2(TIM3) + 1)

#endif

