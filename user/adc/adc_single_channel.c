/**
 * @brief adc采集滑动电位器的模拟值，转换为数字值，并显示在OLED上
 *
 * 硬件连接：GND, VCC分别接GND，3.3v
 *			 中间电压输出接 PA0
**/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"


static void ADC_SingleChannelInit(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// 配置gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AIN;		// 配置为模拟输入
	gpio_cfg.GPIO_Pin = GPIO_Pin_0;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	// 配置adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // adc时钟频率：pclk2/6 = 72/6 = 12MHz
	// 规则组配置为：adc1，adc_channel1, 序列1，采样周期为55个cycle
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef adc_cfg;
	adc_cfg.ADC_ContinuousConvMode = DISABLE;		// 非连续转换
	adc_cfg.ADC_DataAlign = ADC_DataAlign_Right;	// 转换结果右对齐
	adc_cfg.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 无外部触发转换，本例程软件调用
	adc_cfg.ADC_Mode = ADC_Mode_Independent;		// 独立通道转换
	adc_cfg.ADC_NbrOfChannel = 1;
	adc_cfg.ADC_ScanConvMode = DISABLE;				// 非扫描模式
	ADC_Init(ADC1, &adc_cfg);
	
	// 开启adc
	ADC_Cmd(ADC1, ENABLE);
	
	// 校准adc
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

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

