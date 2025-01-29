#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"                  // Device header

// 引脚定义
#define SPI_SIM_PORT			GPIOA
#define SPI_SIM_CS_PIN			GPIO_Pin_4
#define SPI_SIM_CLK_PIN			GPIO_Pin_5
#define SPI_SIM_MISO_PIN		GPIO_Pin_6
#define SPI_SIM_MOSI_PIN		GPIO_Pin_7


void SPIInit(void);
void SPI_Start(void);
void SPI_Stop(void);
uint8_t SPI_SwapByte(uint8_t byte_send);


#endif
