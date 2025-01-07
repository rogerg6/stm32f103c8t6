/**
 * @brief 利用TIM2的的CH2 OC输出比较功能，输出PWM波形
 *		  来控制舵机角度的变化
 *
 * 硬件连接：
 *		舵机正极连接st-link 5v电源，负极接GND
 *		pwm口接PA1，用TIM2的CH2来输入PWM波
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

void PWM_Servo(void)
{	
	/// Servo init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_1;		
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 * 舵机需要频率为50Hz的PWM波
	 * 占空比：2.5%   -90°
	 *  	     5%   -45°
	 *  	   7.5%     0°
	 *  	    10%    45°
	 *  	  12.5%    90°
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 10000 - 1;		// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 144 - 1;		// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;		// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	// 配置OC
	TIM_OCInitTypeDef tim_oc_cfg;
	TIM_OCStructInit(&tim_oc_cfg);
	tim_oc_cfg.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_cfg.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_cfg.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_cfg.TIM_Pulse = 0;					// CCR
	TIM_OC2Init(TIM2, &tim_oc_cfg);
	
	// 使能TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	// Button init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		// 输入上拉
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &gpio_cfg);
	
	// OLED init
	OLED_Init();
	OLED_ShowString(1, 1, "angle:");
	
	int angle = 250;
	
	while (1) {
		
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET) {
			Delay_ms(20);	// 防抖动
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET);
			Delay_ms(20);	// 防抖动
			
			if (angle > 1250)
				angle = 250;
			
			OLED_ShowNum(1, 7, angle, 4);
			TIM_SetCompare2(TIM2, angle);
			angle += 250;
		}
	}
}



