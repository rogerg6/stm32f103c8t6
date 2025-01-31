#include "key.h"
#include "beeper.h"
#include "photoresister.h"


/***
 * @brief 光敏电阻控制蜂鸣器
 *		  暗光响，反之静音。
 *		  蜂鸣器IO口接PB12，光敏电子DO接PB13
 */
void Photoresistor_Beeper(void)
{
	BEEPER_INIT(GPIOB, GPIO_Pin_12);
	PHOTORESISTER_INIT(GPIOB, GPIO_Pin_13);

	while (1) {
		/* 光敏电阻模块是个分压电路，DO上面一个上拉电阻，下面是光敏电阻
		 * 亮的时候阻值低，Do = 0；反之读到Do = 1 
		 */
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET)
			BEEPER_ON(GPIOB, GPIO_Pin_12);  // 响
		else
			BEEPER_OFF(GPIOB, GPIO_Pin_12);
	}
}

