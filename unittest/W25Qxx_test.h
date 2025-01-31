#include "W25Qxx.h"
#include "OLED.h"


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


