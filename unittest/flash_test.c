/**
 * @brief 用flash的最后一页存放掉电不丢失的数据
 * 		  上电读出数据，按下PB0，数据变化，OLED显示，PB10，清空数据
 *        重新上下电，数据依然是下电前的数据
 *
 * 硬件连接
 *		2个按键连接在PB0/10，另一端接地
 *		OLED显示屏
**/

#include "flash.h"
#include "OLED.h"
#include "stm32f10x.h"                  // Device header
#include "key.h"

#define STORE_ADDR		0x0800FC00
#define STORE_SIZE		512
#define INIT_FLAG		0x5A5A

static uint16_t store_data[STORE_SIZE];

static void Store_Save(uint32_t page_addr)
{
	int i;
	
	Flash_ErasePage(page_addr);
	for (i = 0; i < STORE_SIZE; i++)
		Flash_WriteHalfWord(page_addr + i * 2, store_data[i]);
}

static void Store_Clear(uint32_t page_addr)
{
	int i;
	store_data[0] = INIT_FLAG;
	for (i = 1; i < STORE_SIZE; i++)
		store_data[i] = 0;
	Store_Save(page_addr);
}


static void Store_Init(uint32_t page_addr)
{
	int i;
	
	if (Flash_ReadHalfWord(page_addr) != INIT_FLAG) {
		Store_Clear(page_addr);
	} else {
		for (i = 0; i < STORE_SIZE; i++)
			store_data[i] = Flash_ReadHalfWord(page_addr + 2 * i);
	}
}

void Flash_Test(void)
{
	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_0 | GPIO_Pin_10);
	Store_Init(STORE_ADDR);
	
	OLED_ShowString(1, 1, "Flash Test");

	while (1) {
		if (Is_KeyDown(GPIOB, GPIO_Pin_0)) {
			store_data[1] += 1;
			store_data[2] += 2;
			store_data[3] += 3;
			store_data[4] += 4;
			Store_Save(STORE_ADDR);
		}
		if (Is_KeyDown(GPIOB, GPIO_Pin_10)) {
			Store_Clear(STORE_ADDR);
		}
		
		OLED_ShowHexNum(2, 1, store_data[1], 2);
		OLED_ShowHexNum(2, 4, store_data[2], 2);
		OLED_ShowHexNum(3, 1, store_data[3], 2);
		OLED_ShowHexNum(3, 4, store_data[4], 2);
	}
}