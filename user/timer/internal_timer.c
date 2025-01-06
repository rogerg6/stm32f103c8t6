/***
 * @brief 定时器时钟中断 内部时钟
 *        每隔1s，OLED上的显示的数字增加1
 * 
 */
 
#include "internal_timer.h"
#include "OLED.h"



/***
 * @brief 选用TIM2作为通用定时器
 */
static __IO uint32_t tim_cnt = 0;

void Internal_Timer_Test(void)
{
	/// OLED init ///
	OLED_Init();
	OLED_ShowString(1, 1, "cnt = ");
	
	/// Timer init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	/* 配置时基单元。
	 * 计算公式：freq = 72MHz / (Prescaler+1) / (Period+1)
	 * 72MHz / 7200 /  10000 = 1Hz, 1s产生一次中断
	 *
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 10000 - 1;		// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 7200 - 1;	// 预分频数. 
	tim_tb_cfg.TIM_RepetitionCounter = 0;		// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除定时器更新标志位
																//TIM_TimeBaseInit函数末尾，手动产生了更新事件
																//若不清除此标志位，则开启中断后，会立刻进入一次中断
																//如果不介意此问题，则不清除此标志位也可
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
		OLED_ShowNum(1, 7, tim_cnt, 5);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		tim_cnt++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


