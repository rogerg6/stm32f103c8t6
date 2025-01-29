#ifndef __SPI_SIMULATION_H
#define __SPI_SIMULATION_H

#include "stm32f10x.h"                  // Device header

void SPI_SIM_Init(void);
void SPI_SIM_Start(void);
void SPI_SIM_Stop(void);
uint8_t SPI_SIM_SwapByte(uint8_t byte_send);



#endif
