#ifndef __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H
#include "stm32f10x.h"                  // Device header


uint32_t Flash_ReadWord(uint32_t page_addr);
uint16_t Flash_ReadHalfWord(uint32_t page_addr);
uint8_t Flash_ReadByte(uint32_t page_addr);
void Flash_EraseAll(void);
void Flash_ErasePage(uint32_t page_addr);
void Flash_WriteWord(uint32_t page_addr, uint32_t data);
void Flash_WriteHalfWord(uint32_t page_addr, uint16_t data);

#endif

