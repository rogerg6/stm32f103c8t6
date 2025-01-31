/**
 * 旋转编码器计次 测试
 * 旋转编码器上的按钮，正转数字增加，反转数字减少
 *
 * A相接PB0，B相接PB1
 */


#include "rotateEncoder.h"

static __IO uint32_t rotate_cnt = 0;

void RotateEncoder_Init(void)
{
	// 开启时钟, EXIT, NVIC时钟不需要开启
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// GPIO
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		// 看芯片手册
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_cfg);
	
	// AIFO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	// EXTI
	EXTI_InitTypeDef exti_cfg;
	exti_cfg.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	exti_cfg.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_cfg.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_cfg.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_cfg);
	
	// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断分组
	
	NVIC_InitTypeDef nvic_cfg;
	nvic_cfg.NVIC_IRQChannel = EXTI0_IRQn;
	nvic_cfg.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_cfg.NVIC_IRQChannelSubPriority = 1;
	nvic_cfg.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_cfg);
	
	nvic_cfg.NVIC_IRQChannel = EXTI1_IRQn;
	nvic_cfg.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_cfg.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic_cfg);	
}

uint32_t RotateEncoder_GetCount(void)
{
	return rotate_cnt;
}


void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == RESET) {
			// 反转
			rotate_cnt--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == RESET) {
			// 正转
			rotate_cnt++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

