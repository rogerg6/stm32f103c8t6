/**
 * @brief 实现从内存到内存的dma搬运
 *		  data1 -> data2, data1++循环搬运
 *
 * 硬件连接：连接OLED即可，用于显示变量
 */

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"

uint8_t data1[4] = {0x1, 0x2, 0x3, 0x4};
uint8_t data2[4] = {0};
uint16_t datalen = 4;

static void DMAInit(uint32_t src, uint32_t dst, uint16_t size)
{
	// 开启时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// dma init
	DMA_InitTypeDef dma_cfg;
	dma_cfg.DMA_PeripheralBaseAddr = src;
	dma_cfg.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_cfg.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	dma_cfg.DMA_MemoryBaseAddr = dst;
	dma_cfg.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_cfg.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_cfg.DMA_Mode = DMA_Mode_Normal;			// dma不自动加载传输计数器
	dma_cfg.DMA_BufferSize = size;				// 传输计数器的值
	dma_cfg.DMA_DIR = DMA_DIR_PeripheralSRC;	// 传输方向为 periph -> mem
	dma_cfg.DMA_M2M = DMA_M2M_Enable;			// mem2mem, 即软件触发
	dma_cfg.DMA_Priority = DMA_Priority_High;	// dma优先级
	DMA_Init(DMA1_Channel1, &dma_cfg);
}

// dma 开启一次传输
static void DMA_Transfer(void)
{
	// 先disable
	DMA_Cmd(DMA1_Channel1, DISABLE);	
	// 设置传输计数器
	DMA_SetCurrDataCounter(DMA1_Channel1, datalen);
	// 再使能
	DMA_Cmd(DMA1_Channel1, ENABLE);
	// 等待传输完成
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 需要手动清0
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

void DMA_M2M_Test(void)
{
	OLED_Init();
	DMAInit((uint32_t)data1, (uint32_t)data2, datalen);
	
	OLED_ShowString(1, 1, "data1 ");
	OLED_ShowHexNum(1, 7, (uint32_t)data1, 8);
	OLED_ShowHexNum(2, 1,  data1[0], 2);
	OLED_ShowHexNum(2, 4,  data1[1], 2);
	OLED_ShowHexNum(2, 7,  data1[2], 2);
	OLED_ShowHexNum(2, 10, data1[3], 2);

	OLED_ShowString(3, 1, "data2 ");
	OLED_ShowHexNum(3, 7, (uint32_t)data2, 8);
	OLED_ShowHexNum(4, 1,  data2[0], 2);
	OLED_ShowHexNum(4, 4,  data2[1], 2);
	OLED_ShowHexNum(4, 7,  data2[2], 2);
	OLED_ShowHexNum(4, 10, data2[3], 2);
	
	while(1) {
		Delay_s(1);
		DMA_Transfer();
		
		OLED_ShowHexNum(2, 1,  data1[0], 2);
		OLED_ShowHexNum(2, 4,  data1[1], 2);
		OLED_ShowHexNum(2, 7,  data1[2], 2);
		OLED_ShowHexNum(2, 10, data1[3], 2);
		OLED_ShowHexNum(4, 1,  data2[0], 2);
		OLED_ShowHexNum(4, 4,  data2[1], 2);
		OLED_ShowHexNum(4, 7,  data2[2], 2);
		OLED_ShowHexNum(4, 10, data2[3], 2);
		Delay_s(1);
		
		data1[0]++;
		data1[1]++;
		data1[2]++;
		data1[3]++;
		
		OLED_ShowHexNum(2, 1,  data1[0], 2);
		OLED_ShowHexNum(2, 4,  data1[1], 2);
		OLED_ShowHexNum(2, 7,  data1[2], 2);
		OLED_ShowHexNum(2, 10, data1[3], 2);
		OLED_ShowHexNum(4, 1,  data2[0], 2);
		OLED_ShowHexNum(4, 4,  data2[1], 2);
		OLED_ShowHexNum(4, 7,  data2[2], 2);
		OLED_ShowHexNum(4, 10, data2[3], 2);

	}
}

