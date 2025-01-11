/**
 * @brief 利用TIM3的的CH1 IC(输入捕获功能)，对PWM波形进行频率测量
 *		  具体是TIM2 CH1产生频率为1KHz的波形，从PA0输出
 *        TIM3 CH1从PA6读取输入的波形，并通过IC测量输入波形的频率，并在OLED显示结果
 * 硬件连接：
 *		PA0 短接 PA6
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


void PWM_Init(void)
{
	/// PWM init ///
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_0;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 * 配置为频率为1KHz，分辨率为100PWM波
	 *
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 100 - 1;					// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 720 - 1;					// PRC.
	tim_tb_cfg.TIM_RepetitionCounter = 0;				// 高级定时器使用，Tim2是通用定时器，所以设0即可
	TIM_TimeBaseInit(TIM2, &tim_tb_cfg);
	
	// 配置OC，占空比=50%
	TIM_OCInitTypeDef tim_oc_cfg;
	TIM_OCStructInit(&tim_oc_cfg);
	tim_oc_cfg.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_cfg.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_oc_cfg.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc_cfg.TIM_Pulse = 50;					// CCR
	TIM_OC1Init(TIM2, &tim_oc_cfg);
	
	// 使能TIM2
	TIM_Cmd(TIM2, ENABLE);
}

void IC_Init(void)
{
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM3);
	
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_IN_FLOATING;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_6;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 * 配置为频率为1MHz
	 *
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 65536 - 1;					// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 72 - 1;					// PRC.
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


void IC_FreqMeasure(void)
{	
	PWM_Init();
	IC_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "freq=00000Hz");
	
	while (1) {
		OLED_ShowNum(1, 6, 1000000 / TIM_GetCapture1(TIM3), 5);
	}
}


