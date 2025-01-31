#include "adc.h"
#include "OLED.h"
#include "Delay.h"

/**
 * @brief adc采集滑动电位器的模拟值，转换为数字值，并显示在OLED上
 *
 * 硬件连接：GND, VCC分别接GND，3.3v
 *			 中间电压输出接 PA0
**/

void ADC_SingleChannelTest(void)
{
	uint16_t value;
	float voltage;
	
	ADC_SingleChannelInit();
	OLED_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage: 0.00v");
	
	while (1) {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == DISABLE);
		value = ADC_GetConversionValue(ADC1);
		voltage = 3.3 * value / 4095;
		
		OLED_ShowNum(1, 10, value, 4);
		OLED_ShowNum(2, 10, voltage, 1);
		OLED_ShowNum(2, 12, (uint16_t)(voltage * 100) % 100, 2);
		
		Delay_ms(100);
	}
}

/**
 * @brief adc多通道采集的外设模拟值，转换为数字值，并显示在OLED上
 *
 * 硬件连接：
 *		电位器    : GND, VCC分别接GND，3.3v，AO输出接 PA0
 *		光敏传感器: GND, VCC分别接GND，3.3v，AO输出接 PA1
 *		热敏传感器: GND, VCC分别接GND，3.3v，AO输出接 PA2
 *		对射传感器: GND, VCC分别接GND，3.3v，AO输出接 PA3
**/
void ADC_MultiChannelTest(void)
{
	uint16_t ad0, ad1, ad2, ad3;
	
	ADC_SingleChannelInit();
	OLED_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	while (1) {
		ad0 = ADC_GetValue(ADC_Channel_0);
		ad1 = ADC_GetValue(ADC_Channel_1);
		ad2 = ADC_GetValue(ADC_Channel_2);
		ad3 = ADC_GetValue(ADC_Channel_3);
	
		OLED_ShowNum(1, 10, ad0, 4);
		OLED_ShowNum(2, 10, ad1, 4);
		OLED_ShowNum(3, 10, ad2, 4);
		OLED_ShowNum(4, 10, ad3, 4);
		
		Delay_ms(100);
	}
}

