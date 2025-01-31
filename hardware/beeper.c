#include "beeper.h"
#include "Delay.h"
#include "gpio.h"

/**** 
 * 蜂鸣器s
 * IO口接PB12
 */
void Beeper_Test(void)
{
	BEEPER_INIT(GPIOB, GPIO_Pin_12);
		
	while (1) {
		BEEPER_ON(GPIOB, GPIO_Pin_12);		// 低电平响
		Delay_ms(300);
		BEEPER_OFF(GPIOB, GPIO_Pin_12);		// 高电平静音
		Delay_ms(300);
	}
}
