#ifndef __SENSORCOUNTER_H
#define __SENSORCOUNTER_H

#include "stm32f10x.h"                  // Device header


void SensorCounter_Init(void);
uint32_t SensorCounter_GetCount(void);

#endif
