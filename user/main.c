#include "stm32f10x.h"                  // Device header
#include "W25Qxx.h"
#include "OLED.h"

uint8_t MID;
uint16_t DID;

int main(void) 
{
	W25Qxx_Test();
	return 0;
}
