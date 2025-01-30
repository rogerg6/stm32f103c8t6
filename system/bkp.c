/***
 * @brief 备份寄存器的读写，显示在OLED上
 *
 * 硬件连接：stlink的3.3v连接VBAT引脚，给备份寄存器单独供电
 */

#include "stm32f10x.h"                  // Device header
#include "OLED.h"

void BKP_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
}

/**
 * @brief 读写BPK寄存器
 */
void BKP_Test(void)
{
	uint16_t data_write[] = {0x1234, 0x5678};
	uint16_t data_read[2];
	
	OLED_Init();
	BKP_Init();
	
	OLED_ShowString(2, 1, "R");
	
	BKP_WriteBackupRegister(BKP_DR1, data_write[0]);
	BKP_WriteBackupRegister(BKP_DR2, data_write[1]);
	
	OLED_ShowHexNum(2, 3, BKP_ReadBackupRegister(BKP_DR1), 4);
	OLED_ShowHexNum(2, 8, BKP_ReadBackupRegister(BKP_DR2), 4);
		
	while (1);
}

