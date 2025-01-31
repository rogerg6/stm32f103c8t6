#include "adc.h"

void ADC_SingleChannelInit(void)
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

void ADC_MultiChannelInit(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// 配置gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AIN;		// 配置为模拟输入
	gpio_cfg.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	// 配置adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  // adc时钟频率：pclk2/6 = 72/6 = 12MHz
	
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

uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
	// 规则组配置为：adc1，adc_channel1, 序列1，采样周期为55个cycle
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == DISABLE);
	return ADC_GetConversionValue(ADC1);
}

