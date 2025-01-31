#include "flash.h"

// 读flash在page_addr的4字节
uint32_t Flash_ReadWord(uint32_t page_addr)
{
	return *((uint32_t*)page_addr);
}

// 读flash在page_addr的2字节
uint16_t Flash_ReadHalfWord(uint32_t page_addr)
{
	return *((uint16_t*)page_addr);
}

// 读flash在page_addr的1字节
uint8_t Flash_ReadByte(uint32_t page_addr)
{
	return *((uint8_t*)page_addr);
}

// 擦除整个flash
void Flash_EraseAll(void)
{
	FLASH_Unlock();
	FLASH_EraseAllPages();
	FLASH_Lock();
}

// 擦除flash从page_addr开始的一页
void Flash_ErasePage(uint32_t page_addr)
{
	FLASH_Unlock();
	FLASH_ErasePage(page_addr);
	FLASH_Lock();
}

// 写4字节到flash的page_addr
void Flash_WriteWord(uint32_t page_addr, uint32_t data)
{
	FLASH_Unlock();
	FLASH_ProgramWord(page_addr, data);
	FLASH_Lock();	
}

// 写2字节到flash的page_addr
void Flash_WriteHalfWord(uint32_t page_addr, uint16_t data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(page_addr, data);
	FLASH_Lock();	
}