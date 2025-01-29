/***
 * @brief W25Q64 NOR Flash驱动代码
 *
 * 硬件连接：
 *  VCC, GND - 3.3v, GND
 *  PA4 - CS
 *  PA5 - CLK
 *  PA6 - DO
 *  PA7 - DI
 */
#include "spi.h"
#include "OLED.h"
#include "W25Qxx.h"

static void W25Qxx_WaitBusy(void)
{
	uint32_t timeout;
	
	SPI_Start();
	SPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	
	timeout = 0x10000;
	while ((SPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) {
		if (timeout-- == 0)
			break;
	}
	
	SPI_Stop();
}

/**
 * @brief 设置W25Qxx的芯片写使能
 */
static void W25Qxx_WriteEnable(void)
{
	SPI_Start();
	SPI_SwapByte(W25Q64_WRITE_ENABLE);
	SPI_Stop();
	
	W25Qxx_WaitBusy();
}

void W25Qxx_Init(void)
{
	SPIInit();
}

/**
 * @brief 读取W25Qxx的芯片ID号
 *		  MID - 制造商id，DID - 设备id
 */
void W25Qxx_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI_Start();
	SPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= SPI_SwapByte(W25Q64_DUMMY_BYTE);
	SPI_Stop();
}

/**
 * @brief 把data处的len个数据写入地址addr处。
 *        addr最好是page对齐的，len最好小于pagesize - addr%pagesize
 *		  不然会回绕
 */
void W25Qxx_PageProgram(uint32_t addr, uint8_t *data, uint16_t len)
{
	uint16_t i;
	
	W25Qxx_WriteEnable();

	SPI_Start();
	
	SPI_SwapByte(W25Q64_PAGE_PROGRAM);
	SPI_SwapByte(addr >> 16);
	SPI_SwapByte(addr >> 8);
	SPI_SwapByte(addr);
	
	for (i = 0; i < len; i++) {
		SPI_SwapByte(data[i]);
	}
	
	SPI_Stop();
	W25Qxx_WaitBusy();
}

/**
 * @brief 把地址addr处开始的一页擦除
 */
void W25Qxx_SectorErase(uint32_t addr)
{
	W25Qxx_WriteEnable();

	SPI_Start();
	
	SPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	SPI_SwapByte(addr >> 16);
	SPI_SwapByte(addr >> 8);
	SPI_SwapByte(addr);
	
	SPI_Stop();
	W25Qxx_WaitBusy();
}

/**
 * @brief 把addr处的len个数据读到data。
 */
void W25Qxx_ReadData(uint32_t addr, uint8_t *data, uint32_t len)
{
	uint32_t i;

	SPI_Start();
	
	SPI_SwapByte(W25Q64_READ_DATA);
	SPI_SwapByte(addr >> 16);
	SPI_SwapByte(addr >> 8);
	SPI_SwapByte(addr);
	
	for (i = 0; i < len; i++) {
		data[i] = SPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	SPI_Stop();
}

void W25Qxx_Test(void)
{
	uint8_t MID;
	uint16_t DID;
	uint8_t data_write[] = {0x11, 0x22, 0x33, 0x44};
	uint8_t data_read[4] = {0};
	
	OLED_Init();
	W25Qxx_Init();
	
	OLED_ShowString(1, 1, "MID    DID");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");

	W25Qxx_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	
	// 写入数据
	W25Qxx_SectorErase(0x000000);
	W25Qxx_PageProgram(0x000000, data_write, 4);
	// 读出数据
	W25Qxx_ReadData(0x000000, data_read, 4);
	
	// 显示结果
	OLED_ShowHexNum(2, 3, data_write[0], 2);
	OLED_ShowHexNum(2, 6, data_write[1], 2);
	OLED_ShowHexNum(2, 9, data_write[2], 2);
	OLED_ShowHexNum(2, 12, data_write[3], 2);
	
	OLED_ShowHexNum(3, 3, data_read[0], 2);
	OLED_ShowHexNum(3, 6, data_read[1], 2);
	OLED_ShowHexNum(3, 9, data_read[2], 2);
	OLED_ShowHexNum(3, 12, data_read[3], 2);

	while (1);
}


