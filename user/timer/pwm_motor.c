/**
 * @brief 利用TIM2的的CH3 OC输出比较功能，输出PWM波形
 *		  来, 通过按键控制直流电机的旋转速度、方向
 *
 * 硬件连接：
 *      电机控制芯片
 *			STANDBY 接3.3v
 *			VCC，GND分别接3.3v，GND
 *			VM接st-link 5v电源
 *			PWMA口接PA2
 *			AIN1, AIN2分别接PA4，PA5
 *		电机正极，负极接点击控制芯片AO1, AO2
 *		按键接PB10
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

void PWM_Motor(void)
{	
	/// Servo init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_cfg;
	
	// 电机控制芯片方向GPIO配置
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 复用推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_2;		
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 * 舵机需要频率为20KHz的PWM波
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 100 - 1;		// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 36 - 1;		// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;		// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	// 配置OC
	TIM_OCInitTypeDef tim_oc_cfg;
	TIM_OCStructInit(&tim_oc_cfg);
	tim_oc_cfg.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_cfg.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_cfg.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_cfg.TIM_Pulse = 0;					// CCR
	TIM_OC3Init(TIM2, &tim_oc_cfg);
	
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
	OLED_ShowString(1, 1, "speed:");
	
	int speed = 0;
	while (1) {
		OLED_ShowSignedNum(1, 8, speed, 4);
		
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET) {
			Delay_ms(20);	// 防抖动
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET);
			Delay_ms(20);	// 防抖动
			
			speed += 20;
			
			if (speed > 100)
				speed = -100;
			
			if (speed > 0) {
				GPIO_SetBits(GPIOA, GPIO_Pin_4);
				GPIO_ResetBits(GPIOA, GPIO_Pin_5);
				TIM_SetCompare3(TIM2, speed);
			}
			else {
				GPIO_ResetBits(GPIOA, GPIO_Pin_4);
				GPIO_SetBits(GPIOA, GPIO_Pin_5);
				TIM_SetCompare3(TIM2, -speed);
			}
			
		}
	}
}



