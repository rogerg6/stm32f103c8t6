#include "pwm.h"
#include "gpio.h"

static __IO uint16_t g_duty;
static __IO uint16_t g_cycle;

/**
 * @brief 用TIM2通用定时器Channel1，选择内部时钟源产生PWM波
 * @param freq    频率
 * @param cycle   单个周期计数值
 * @param duty    高电平计数值
 **/
void PWM_Init(uint32_t freq, uint16_t cycle, uint16_t duty)
{
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	// GPIO配置
	GPIO_INIT(GPIOA, GPIO_Pin_0, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	
	/* 配置时基单元。
	 * freq = 72000000 / (TIM_Prescaler * TIM_Period)
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;		// 递增计数
	tim_tb_cfg.TIM_Period = cycle - 1;						// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = (72000000 / freq / cycle) - 1;		// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;				// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	// 配置OC，占空比
	TIM_OCInitTypeDef tim_oc_cfg;
	TIM_OCStructInit(&tim_oc_cfg);
	tim_oc_cfg.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_cfg.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_cfg.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_cfg.TIM_Pulse = duty;						// CCR
	TIM_OC1Init(TIM2, &tim_oc_cfg);
	
	// 使能TIM2
	TIM_Cmd(TIM2, ENABLE);
	g_duty = duty;
	g_cycle = cycle;
}

void PWM_SetDuty(float dr)
{
	if (dr < 0 || dr > 100)
		return;
	TIM_SetCompare1(TIM2, (uint16_t)(dr * g_cycle));
}
