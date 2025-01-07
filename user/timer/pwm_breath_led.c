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


void PWM_Breath_Led(void)
{
	int i;
	
	/// PWM init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	// GPIO重映射. 把TIM2_CH1映射到PA15
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_0;		// GPIO_Pin_15
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 * 配置为频率为1KHz，分辨率为100的PWM波
	 *
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 100 - 1;		// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 720 - 1;		// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;		// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	// 配置OC
	TIM_OCInitTypeDef tim_oc_cfg;
	TIM_OCStructInit(&tim_oc_cfg);
	tim_oc_cfg.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_cfg.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_cfg.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_cfg.TIM_Pulse = 0;					// CCR
	TIM_OC1Init(TIM2, &tim_oc_cfg);
	
	// 使能TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	while (1) {
		for (i = 0; i <= 100; i++) {
			TIM_SetCompare1(TIM2, i);
			Delay_ms(10);
		}
		for (i = 0; i <= 100; i++) {
			TIM_SetCompare1(TIM2, 100 - i);
			Delay_ms(10);
		}
	}
}


