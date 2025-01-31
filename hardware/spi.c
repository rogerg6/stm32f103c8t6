/***
 * @brief 软硬件实现SPI通信, 采用spi mode0
 *
 *  A4 - cs
 *  A5 - clk
 *  A6 - MISO
 *  A7 - MOSI
 */
#include "spi.h"

#define USE_SPI_SIMULATION 0


static void CS_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_CS_PIN, (BitAction)x);
}

#if USE_SPI_SIMULATION

static void CLK_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_CLK_PIN, (BitAction)x);
}

static void MOSI_Set(uint8_t x)
{
	GPIO_WriteBit(SPI_SIM_PORT, SPI_SIM_MOSI_PIN, (BitAction)x);
}

static uint8_t MISO_Get(void)
{
	return GPIO_ReadInputDataBit(SPI_SIM_PORT, SPI_SIM_MISO_PIN);
}

void SPIInit(void)
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

void SPI_Start(void)
{
	CS_Set(0);
}

void SPI_Stop(void)
{
	CS_Set(1);
}

// spi mode0的移位时序
uint8_t SPI_SwapByte(uint8_t byte_send)
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

#else
void SPI_Start(void)
{
	CS_Set(0);
}

void SPI_Stop(void)
{
	CS_Set(1);
}

void SPIInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_cfg.GPIO_Pin = SPI_SIM_CS_PIN;		// ss引脚软件模拟
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SIM_PORT, &gpio_cfg);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_cfg.GPIO_Pin = SPI_SIM_CLK_PIN | SPI_SIM_MOSI_PIN;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SIM_PORT, &gpio_cfg);

	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
	gpio_cfg.GPIO_Pin = SPI_SIM_MISO_PIN;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_SIM_PORT, &gpio_cfg);
	
	SPI_InitTypeDef spi_cfg;
	spi_cfg.SPI_Mode = SPI_Mode_Master;
	spi_cfg.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_cfg.SPI_DataSize = SPI_DataSize_8b;
	spi_cfg.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_cfg.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	spi_cfg.SPI_CPOL = SPI_CPOL_Low;		// CPOL=0
	spi_cfg.SPI_CPHA = SPI_CPHA_1Edge;		// CPHA=0
	spi_cfg.SPI_NSS = SPI_NSS_Soft;			// ss引脚软件模拟
	spi_cfg.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &spi_cfg);
	
	SPI_Cmd(SPI1, ENABLE);
	
	// spi mode 0
	CS_Set(1);
}

uint8_t SPI_SwapByte(uint8_t byte_send)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	SPI_I2S_SendData(SPI1, byte_send);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_I2S_ReceiveData(SPI1);
}



#endif

