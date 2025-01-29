#include "stm32f10x.h"                  // Device header
#include "W25Qxx.h"
#include "OLED.h"

int main(void) 
{
	uint8_t MID;
	uint16_t DID;

	W25Qxx_Test();
	return 0;
}
