/**
 * @brief 利用TIM2的的CH1 OC输出比较功能，输出PWM波形
 *		  来控制led的亮度变化，实现呼吸灯的功能
 *
 * 硬件连接：
 *		无重映射：led灯正极连接PA0，负极接GND
 *		重映射  ：led灯正极连接PA15，负极接GND
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "pwm.h"


void BreathLed_Test(void)
{
	int i;
	
	PWM_Init(1000, 100, 0);	
	while (1) {
		for (i = 0; i <= 100; i++) {
			PWM_SetDuty(i/100.0);
			Delay_ms(15);
		}
		for (i = 100; i >=0; i--) {
			PWM_SetDuty(i/100.0);
			Delay_ms(15);
		}
	}
}


