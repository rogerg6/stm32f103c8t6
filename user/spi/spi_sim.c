/***
 * @brief 用gpio软件模拟SPI通信, 采用spi mode0
 *
 *  A4 - cs
 *  A5 - clk
 *  A6 - MISO
 *  A7 - MOSI
 */
#include "spi_sim.h"

#define SPI_SIM_PORT		GPIOA
#define SPI_SIM_CS_PIN			GPIO_Pin_4
#define SPI_SIM_CLK_PIN			GPIO_Pin_5
#define SPI_SIM_MISO_PIN		GPIO_Pin_6
#define SPI_SIM_MOSI_PIN		GPIO_Pin_7

void CS_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_CS_PIN, (BitAction)x);
}

void CLK_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_CLK_PIN, (BitAction)x);
}

void MOSI_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_MOSI_PIN, (BitAction)x);
}

uint8_t MISO_Get(void)
{
	return GPIO_ReadInputDataBit(SPI_SIM_PORT, SPI_SIM_MISO_PIN);
}

void SPI_SIM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_cfg.GPIO_Pin = SPI_SIM_CS_PIN | SPI_SIM_CLK_PIN | SPI_SIM_MOSI_PIN;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SIM_PORT, &gpio_cfg);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
	gpio_cfg.GPIO_Pin = SPI_SIM_MISO_PIN;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SIM_PORT, &gpio_cfg);
	
	// spi mode 0
	CS_Set(1);
	CLK_Set(0);
}

void SPI_SIM_Start(void)
{
	CS_Set(0);
}

void SPI_SIM_Stop(void)
{
	CS_Set(1);
}

// spi mode0的移位时序
uint8_t SPI_SIM_SwapByte(uint8_t byte_send)
{
	uint8_t byte_recv = 0x00;
	int i;
	
	for (i = 0; i < 8; i++) {
		MOSI_Set(!!(byte_send & (0x80 >> i)));		// clk下降沿，移出数据
		CLK_Set(1);
		if (MISO_Get()) { 
			byte_recv |= (0x80 >> i);				// clk下降沿，采样数据
		}
		CLK_Set(0);
	}
	return byte_recv;
	
}

