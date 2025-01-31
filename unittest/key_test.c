#include "key.h"
#include "gpio.h"
#include "led.h"
#include "Delay.h"

/**
 * @brief 按键控制led灯, key1/2控制led1/2
 * 		  LED    一极分别接到PA0,PA1,  一极接GND
 * 		  Button 一极分别接到PB0,PB10, 一极接GND
 */
void Key_Test(void)
{
	LED_INIT(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_Write(GPIOA, 0xffff);		// 全灭
	
	KEY_INIT(GPIOB, GPIO_Pin_0 | GPIO_Pin_10);
	
	while (1) {
		if (Is_KeyDown(GPIOB, GPIO_Pin_0)) {
			GPIO_TOGGLE(GPIOA, GPIO_Pin_0);
		}
		if (Is_KeyDown(GPIOB, GPIO_Pin_10)) {
			GPIO_TOGGLE(GPIOA, GPIO_Pin_1);
		}
	}
}

