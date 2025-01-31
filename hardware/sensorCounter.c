/**
 * 对射式红外传感器计次 测试
 * 遮挡红外传感器，OLED上显示遮挡的次数。
 *
 * DO接PB14
 */
#include "sensorCounter.h"

static __IO uint32_t g_sensor_cnt = 0;

void SensorCounter_Init(void)
{
	// 开启时钟, EXIT, NVIC时钟不需要开启
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// GPIO
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		// 看芯片手册
	gpio_cfg.GPIO_Pin = GPIO_Pin_14;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_cfg);
	
	// AIFO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// EXTI
	EXTI_InitTypeDef exti_cfg;
	exti_cfg.EXTI_Line = EXTI_Line14;
	exti_cfg.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_cfg.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_cfg.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_cfg);
	
	// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断分组
	
	NVIC_InitTypeDef nvic_cfg;
	nvic_cfg.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic_cfg.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_cfg.NVIC_IRQChannelSubPriority = 1;
	nvic_cfg.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_cfg);	
}

uint32_t SensorCounter_GetCount(void)
{
	return g_sensor_cnt;
}

// SensorCounter DO 中断handler
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET) {
		// 防止数字乱跳，进入中断再次读取IO的值
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET)
			g_sensor_cnt++;
		EXTI_ClearITPendingBit(EXTI_Line14);	// 中断处理后，必须清除中断，否则会一直触发中断
	}
}
