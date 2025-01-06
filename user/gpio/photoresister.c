#include "button.h"


/***
 * @brief 光敏电阻控制蜂鸣器
 *		  暗光响，反之静音。
 *		  蜂鸣器IO口接PB12，光敏电子DO接PB13
 */
void Photoresistor_Beeper(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	/**** beeper init *******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_12;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &gpio_cfg);
	
	/**** Photoresistor init *******/
	gpio_cfg.GPIO_Mode = GPIO_Mode_IN_FLOATING;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_13;	
	GPIO_Init(GPIOB, &gpio_cfg);

	while (1) {
		/* 光敏电阻模块是个分压电路，DO上面一个上拉电阻，下面是光敏电阻
		 * 亮的时候阻值低，Do = 0；反之读到Do = 1 
		 */
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET)
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);  // 响
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
}

