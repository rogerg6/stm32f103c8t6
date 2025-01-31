/**
 * @brief 利用TIM2的的CH1 OC输出比较功能，输出PWM波形
 *		  ，通过按键控制舵机角度的变化
 *
 * 硬件连接：
 *		舵机
 *			正极连接st-link 5v电源，负极接GND
 *			PWM口接PA0，用TIM2的CH2来输入PWM波
 *		按键接PB10
 *
 */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "pwm.h"

void Servo_Test(void)
{	
	/* 配置时基单元。
	 * 舵机需要频率为50Hz的PWM波
	 * 占空比：2.5%   -90°
	 *  	     5%   -45°
	 *  	   7.5%     0°
	 *  	    10%    45°
	 *  	  12.5%    90°
	 */
	PWM_Init(50, 10000, 0);
	
	KEY_INIT(GPIOB, GPIO_Pin_10);
	OLED_Init();
	
	OLED_ShowString(1, 1, "angle:");
	
	float duty = 0.025;
	
	while (1) {
		
		if (Is_KeyDown(GPIOB, GPIO_Pin_10)) {
			if (duty > 0.125)
				duty = 0.025;
			
			OLED_ShowNum(1, 7, duty*10000, 4);
			PWM_SetDuty(duty);
			duty += 0.025;
		}
	}
}



