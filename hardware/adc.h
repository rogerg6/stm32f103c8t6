#ifndef __MY_ADC_H
#define __MY_ADC_H

#include "stm32f10x.h"                  // Device header

void ADC_SingleChannelInit(void);
void ADC_MultiChannelInit(void);
uint16_t ADC_GetValue(uint8_t ADC_Channel);


#endif

