/**
 * @brief 编码器测速
 * 		  旋转编码器上的按钮，旋转的越快，OLED显示的速度越大
 *		  正转数字变大，翻转数字变小
 *
 * 应用场景：给运行中的电机测速，一般电机会自带正交编码器，当电机
 *			 转动的时候，编码器会输出正交的波形，反映其旋转方向和速度。
 *			 本例程通过，旋转编码器模拟产生正交波形，测量手动旋转的
 *			 方向和速度
 *
 * 旋转编码器硬件连接：
 *		VCC, GND分别接3.3v，GND
 *		A相接PA6，B相接PA7
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

static __IO int16_t speed;


static void Encoder_Init(void)
{
	// 使能系统时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// 无需配置TIM3的时钟为内部时钟，因为是从编码器获取时钟周期的
		
	// GPIO配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	/* 配置时基单元。
	 *
	 */
	TIM_TimeBaseInitTypeDef tim_tb_cfg;
	tim_tb_cfg.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_tb_cfg.TIM_CounterMode = TIM_CounterMode_Up;	// 递增计数
	tim_tb_cfg.TIM_Period = 65536 - 1;					// ARR寄存器的值
	tim_tb_cfg.TIM_Prescaler = 1 - 1;					// PRC. 时钟来自编码器的旋转，所以不分频
	tim_tb_cfg.TIM_RepetitionCounter = 0;			
	TIM_TimeBaseInit(TIM3, &tim_tb_cfg);
	
	// 配置IC
	TIM_ICInitTypeDef tim_ic_cfg;
	TIM_ICStructInit(&tim_ic_cfg);
	
	tim_ic_cfg.TIM_Channel = TIM_Channel_1;
	tim_ic_cfg.TIM_ICFilter = 0x0F;
	TIM_ICInit(TIM3, &tim_ic_cfg);
	
	tim_ic_cfg.TIM_Channel = TIM_Channel_2;
	tim_ic_cfg.TIM_ICFilter = 0x0F;
	TIM_ICInit(TIM3, &tim_ic_cfg);

	
	/* 配置编码器接口
	 * TIM_EncoderMode_TI12: AB相上下边沿都触发计次
	 * TIM_ICPolarity_Rising/Falling：Rising代表极性不反转，Falling代表翻转极性
	 */
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}

void TimerInit(void)
{	
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
}

void Encoder_Speed_Measure(void)
{
	TimerInit();
	Encoder_Init();
	OLED_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	/**
	 * 测速，每秒旋转的次数
	**/
	while (1) {
		OLED_ShowSignedNum(1, 7, speed, 5);
		//TIM_SetCounter(TIM3, 0);
		//Delay_ms(1000);
	}
}

#if 0		// redefine

// 利用定时中断来算单位时间的转动次数，即旋转速度
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		speed = (int16_t)TIM_GetCounter(TIM3);
		TIM_SetCounter(TIM3, 0);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
#endif

