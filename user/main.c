//#include "stm32f10x.h"                  // Device header
typedef unsigned int uint32_t;

#define RCC_APB2ENR		(*((uint32_t*)(0x40021000 + 0x18)))
#define GPIOC_CRH		(*((uint32_t*)(0x40011000 + 0x04)))
#define GPIOC_BSRR		(*((uint32_t*)(0x40011000 + 0x10)))
#define GPIOC_ODR		(*((uint32_t*)(0x40011000 + 0x0C)))

int main(void) 
{
	RCC_APB2ENR = (1 << 4);				// 开启gpioc时钟
	GPIOC_CRH = (0<<22) | (1<<20);		// 输出模式，推挽输出
	GPIOC_BSRR = (1<<29);				// 输出低电平
	
	//RCC->APB2ENR = 0x10;
	//GPIOC->CRH = 0x300000;
	//GPIOC->ODR = 0x0;
	while (1);
	return 0;
}
