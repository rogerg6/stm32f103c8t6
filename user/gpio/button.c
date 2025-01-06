#include "button.h"
#include "Delay.h"
#include "gpio.h"




/**
 * @brief 按键控制led灯, key1/2控制led1/2
 * 		  LED    一极分别接到PA0,PA1,  一极接GND
 * 		  Button 一极分别接到PB0,PB10, 一极接GND
 */
void Button_Test(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	/********* led init *********/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &gpio_cfg);
	GPIO_Write(GPIOA, 0xffff);		// 全灭
	
	/******** button init *********/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		// 输入上拉
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &gpio_cfg);
	
	while (1) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET) {
			Delay_ms(20);	// 防抖动
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET);
			Delay_ms(20);	// 防抖动
			Gpio_Pin_Toggle(GPIOA, GPIO_Pin_0);
		}
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET) {
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET);
			Delay_ms(20);
			Gpio_Pin_Toggle(GPIOA, GPIO_Pin_1);
		}
	}
}

