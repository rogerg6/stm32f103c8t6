/***
 * @brief 定时器时钟中断 外部时钟
 * 
 * 硬件连接：对冲式红外传感器的DO接PA0
 *
 * 用对冲式红外传感器的DO引脚作为外部时钟源，每遮挡松开
 * 一次可以认为是一个时钟脉冲。每个时钟周期tim_cnt++, 当
 * tim_cnt==10的时候，cnt++
 * 
 */
 

#include "external_timer.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"



/***
 * @brief 选用TIM2作为通用定时器
 */
static __IO uint32_t cnt = 0;


void External_Timer_Test(void)
{
	/// OLED init ///
	OLED_Init();
	OLED_ShowString(1, 1, "cnt = ");
	OLED_ShowString(2, 1, "tim_cnt = ");

	
	/// Timer init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 配置PA0为外部时钟源
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
	gpio_cfg.GPIO_Pin = GPIO_Pin_0;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/**
	 * 选择外部时钟作为时钟源.
	 * 不分频
	 * 最后一个滤波器参数加到最大0x0F，可滤除时钟信号抖动
	 */
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	// 配置时基单元。
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 10 - 1;		// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 1 - 1;	    // 预分频数
	tim_tb_cfg.TIM_RepetitionCounter = 0;		// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	
	// 配置中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef nvic_cfg;
	nvic_cfg.NVIC_IRQChannel = TIM2_IRQn;
	nvic_cfg.NVIC_IRQChannelCmd = ENABLE;
	nvic_cfg.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_cfg.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_cfg);
	
	// 使能TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	while (1) {
		OLED_ShowNum(1, 7, cnt, 5);
		OLED_ShowNum(2, 11, TIM_GetCounter(TIM2), 5);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		cnt++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}





