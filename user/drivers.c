#include "drivers.h"

// led闪烁。核心板上的D2
void Led_Blink(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_13;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOC, &gpio_cfg);
	
	
	while (1)
	{
		// 点亮led
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);	
		Delay_ms(500);
		// 关闭led
		GPIO_SetBits(GPIOC, GPIO_Pin_13);	
		Delay_ms(500);
	}
	
}

/***
 * @brief led流水灯. 8个led负极分别接到PA0~7, 正极接VCC
 * @parma ms延迟时间
 */
void Led_TubeLight(uint32_t ms)
{
	GPIO_InitTypeDef gpio_cfg;
	int i;
	uint16_t val;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |\
						GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | \
						GPIO_Pin_6 | GPIO_Pin_7;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOA, &gpio_cfg);
	
	// 全亮全灭
	GPIO_Write(GPIOA, 0x0);
	Delay_ms(10 * ms);
	GPIO_Write(GPIOA, 0xffff);
	
	while (1) {
		val = 1;
		for (i = 0; i < 8; i++) {
			GPIO_WriteBit(GPIOA, val, Bit_RESET);	
			Delay_ms(ms);
			GPIO_WriteBit(GPIOA, val, Bit_SET);	
			Delay_ms(ms);
			val <<= 1;
		}
	}
	
}

/**** 
 * 蜂鸣器
 * IO口接PB12
 */
void Beeper_Test(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	// 开启gpioc时钟, gpioc属于APB2总线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		// 通用输出模式，推挽输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_12;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		// gpio切换频率
	GPIO_Init(GPIOB, &gpio_cfg);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	
	while (1) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);	// 低电平响
		Delay_ms(300);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);		// 高电平静音
		Delay_ms(300);
	}
}

/**
 * @brief 按键控制led灯, key1/2控制led1/2
 * 		  LED    一极分别接到PA0,PA1,  一极接GND
 * 		  Button 一极分别接到PB0,PB10, 一极接GND
 */
void Button_Test(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	/********* led init *********/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &gpio_cfg);
	GPIO_Write(GPIOA, 0xffff);		// 全灭
	
	/******** button init *********/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;		// 输入上拉
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_10;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &gpio_cfg);
	
	while (1) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET) {
			Delay_ms(20);	// 防抖动
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET);
			Delay_ms(20);	// 防抖动
			Gpio_Pin_Toggle(GPIOA, GPIO_Pin_0);
		}
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET) {
			Delay_ms(20);
			while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET);
			Delay_ms(20);
			Gpio_Pin_Toggle(GPIOA, GPIO_Pin_1);
		}
	}
}

/***
 * @brief 光敏电阻控制蜂鸣器
 *		  暗光响，反之静音。
 *		  蜂鸣器IO口接PB12，光敏电子DO接PB13
 */
void Photoresistor_Beeper(void)
{
	GPIO_InitTypeDef gpio_cfg;
	
	/**** beeper init *******/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_PP;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_12;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &gpio_cfg);
	
	/**** Photoresistor init *******/
	gpio_cfg.GPIO_Mode = GPIO_Mode_IN_FLOATING;		
	gpio_cfg.GPIO_Pin = GPIO_Pin_13;	
	GPIO_Init(GPIOB, &gpio_cfg);

	while (1) {
		/* 光敏电阻模块是个分压电路，DO上面一个上拉电阻，下面是光敏电阻
		 * 亮的时候阻值低，Do = 0；反之读到Do = 1 
		 */
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET)
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);  // 响
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
}

/** OLED测试 
 * VCC, GND, SCL, SDA分别接B6 ~ B9
* 该模块以后用于debug显示变量使用，硬件电路接法一般不会变化
 **/
void OLED_Test(void)
{
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Hello World");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -66, 2);
	OLED_ShowHexNum(3, 1, 0xAA55, 4);
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while(1);
}

////////////// 对射式红外传感器计次 ////////////////
/**
 * 对射式红外传感器计次 测试
 * 遮挡红外传感器，OLED上显示遮挡的次数。
 *
 * DO接PB14
 */
static __IO uint32_t sensor_cnt = 0;

void SensorCounter_Test(void)
{
	////////// Sensor Counter init ///////////
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
	
	////////// OLED init ///////////
	OLED_Init();
	OLED_ShowString(1, 1, "cnt = ");
	
	while (1) {
		OLED_ShowNum(1, 7, sensor_cnt, 5);
	}
}

// SensorCounter DO 中断handler
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET) {
		// 防止数字乱跳，进入中断再次读取IO的值
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET)
			sensor_cnt++;
		EXTI_ClearITPendingBit(EXTI_Line14);	// 中断处理后，必须清除中断，否则会一直触发中断
	}
}
////////////// 对射式红外传感器计次 ////////////////



////////////// 旋转编码器计次 ////////////////////////
/**
 * 旋转编码器计次 测试
 * 旋转编码器上的按钮，正转数字增加，反转数字减少
 *
 * A相接PB0，B相接PB1
 */
static __IO uint32_t rotate_cnt = 0;

void Rotate_Test(void)
{
	////////// Sensor Counter init ///////////
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
	
	////////// OLED init ///////////
	OLED_Init();
	OLED_ShowString(1, 1, "cnt = ");
	
	while (1) {
		OLED_ShowSignedNum(1, 7, rotate_cnt, 5);
	}
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
////////////// 旋转编码器计次 ////////////////////////




