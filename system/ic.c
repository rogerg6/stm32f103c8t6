#include "ic.h"
#include "gpio.h"

/**
 * @brief 用TIM3通用定时器Channel1，作为输入捕获。
 *		  一般用测量输入波形频率和pwm占空比
 * @param freq      采样频率
 * @param is_pwm    1测试频率和占空比，0只测频率
 **/
void IC3_1_Init(uint32_t freq, uint8_t is_pwm)
{
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// GPIO配置
	GPIO_INIT(GPIOA, GPIO_Pin_6, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM3);
		
	/* 配置时基单元。
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 65536 - 1;					// ARR寄存器的值. 设置为最大，防止溢出
	tim_tb_cfg.TIM_Prescaler = (72000000 / freq) - 1;	// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &tim_tb_cfg);

	/**
	 * 配置IC
	 */
	TIM_ICInitTypeDef tim_ic_cfg;
	tim_ic_cfg.TIM_Channel = TIM_Channel_1;
	tim_ic_cfg.TIM_ICFilter = 0xF;
	tim_ic_cfg.TIM_ICPolarity = TIM_ICPolarity_Rising;
	tim_ic_cfg.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	tim_ic_cfg.TIM_ICSelection = TIM_ICSelection_DirectTI;
	if (is_pwm)
		TIM_PWMIConfig(TIM3, &tim_ic_cfg);
	else
		TIM_ICInit(TIM3, &tim_ic_cfg);
	/**
	 * 配置输入触发源，从模式。可以实现自动清零计数器，这样读取的CCR的值
	 * 就是一个周期内经过的CNT数。软件只需要读取CCR的值即可获得周期数，其余都是硬件
	 * 自动执行
	 */
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);
}
