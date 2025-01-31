/**
 * @brief 测周法测频率和占空比
 * 		  利用TIM3的的CH1 IC(输入捕获功能)，对PWM波形进行频率, 占空比测量
 *		  具体是TIM2 CH1产生PWM波形，从PA0输出
 *        TIM3 CH1从PA6读取输入的波形，并通过IC测量输入波形的频率，占空比，并在OLED显示结果
 * 硬件连接：
 *		PA0 短接 PA6
 *
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "pwm.h"
#include "ic.h"


void PWMMeasure_Test(void)
{	
	PWM_Init(1000, 100, 50);
	IC3_1_Init(1000000, 1);
	OLED_Init();
	
	OLED_ShowString(1, 1, "freq=00000Hz");
	OLED_ShowString(2, 1, "duty=00%");
	
	//PWM_SetDuty(0.5);
	while (1) {
		OLED_ShowNum(1, 6, 1000000 / IC3_1_GetCount(), 5);
		OLED_ShowNum(2, 6, 100 * IC3_1_GetDutyCount() / IC3_1_GetCount(), 2);
	}
}


