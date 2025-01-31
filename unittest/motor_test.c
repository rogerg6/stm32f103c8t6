/**
 * @brief 利用TIM2的的CH1 OC输出比较功能，输出PWM波形
 *		  来, 通过按键控制直流电机的旋转速度、方向
 *
 * 硬件连接：
 *      电机控制芯片
 *			STANDBY 接3.3v
 *			VCC，GND分别接3.3v，GND
 *			VM接st-link 5v电源
 *			PWMA口接PA0
 *			AIN1, AIN2分别接PA4，PA5
 *		电机正极，负极接点击控制芯片AO1, AO2
 *		按键接PB10
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "gpio.h"
#include "pwm.h"
#include "key.h"

void Motor_Test(void)
{
	// 电机初始化
	PWM_Init(20000, 100, 0);	
	GPIO_INIT(GPIOA, GPIO_Pin_4 | GPIO_Pin_5, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);	// 电机控制芯片方向GPIO配置
	
	KEY_INIT(GPIOB, GPIO_Pin_10);
	OLED_Init();
	
	OLED_ShowString(1, 1, "speed:");
	
	float speed = 0;
	while (1) {
		OLED_ShowSignedNum(1, 8, speed, 4);
		
		if (Is_KeyDown(GPIOB, GPIO_Pin_10)) {
			speed += 0.2;
			
			if (speed > 1)
				speed = -1;
			
			if (speed > 0) {
				GPIO_ON(GPIOA, GPIO_Pin_4);
				GPIO_OFF(GPIOA, GPIO_Pin_5);
				PWM_SetDuty(speed);
			}
			else {
				GPIO_OFF(GPIOA, GPIO_Pin_4);
				GPIO_ON(GPIOA, GPIO_Pin_5);
				PWM_SetDuty(-speed);
			}
			
		}
	}
}



