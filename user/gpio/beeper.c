#include "beeper.h"
#include "Delay.h"

/**** 
 * 蜂鸣器
 * IO口接PB12
 */
void Beeper_Test(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_12;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOB, &gpio_cfg);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	
	while (1) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);	// 低电平响
		Delay_ms(300);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);		// 高电平静音
		Delay_ms(300);
	}
}
