/**
 * @brief adc多通道单次扫描 + DMA单次传输模式，采集外设模拟值
 *
 * 硬件连接：
 *		电位器    : GND, VCC分别接GND，3.3v，AO输出接 PA0
 *		光敏传感器: GND, VCC分别接GND，3.3v，AO输出接 PA1
 *		热敏传感器: GND, VCC分别接GND，3.3v，AO输出接 PA2
 *		对射传感器: GND, VCC分别接GND，3.3v，AO输出接 PA3
**/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"

uint16_t AD_values[4];

static void DMAInit(uint32_t src, uint32_t dst, uint16_t size)
{
	// 开启时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// dma init
	DMA_InitTypeDef dma_cfg;
	dma_cfg.DMA_PeripheralBaseAddr = src;
	dma_cfg.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_cfg.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_cfg.DMA_MemoryBaseAddr = dst;
	dma_cfg.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_cfg.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_cfg.DMA_Mode = DMA_Mode_Normal;			// dma不自动加载传输计数器
	dma_cfg.DMA_BufferSize = size;				// 传输计数器的值
	dma_cfg.DMA_DIR = DMA_DIR_PeripheralSRC;	// 传输方向为 periph -> mem
	dma_cfg.DMA_M2M = DMA_M2M_Disable;			// 硬件触发，本例程是adc转换完成触发dma搬运
	dma_cfg.DMA_Priority = DMA_Priority_High;	// dma优先级
	DMA_Init(DMA1_Channel1, &dma_cfg);
}


static void ADC_SingleChannelInit(void)
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
	
	// 4个序列对应4个通道(PA0/1/2/3)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);

	ADC_InitTypeDef adc_cfg;
	adc_cfg.ADC_ContinuousConvMode = DISABLE;		// 非连续转换
	adc_cfg.ADC_DataAlign = ADC_DataAlign_Right;	// 转换结果右对齐
	adc_cfg.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 无外部触发转换，本例程软件调用
	adc_cfg.ADC_Mode = ADC_Mode_Independent;		// 独立通道转换
	adc_cfg.ADC_NbrOfChannel = 4;
	adc_cfg.ADC_ScanConvMode = ENABLE;				// 扫描模式
	ADC_Init(ADC1, &adc_cfg);
	
	// 配置dma
	DMAInit((uint32_t)&ADC1->DR, (uint32_t)AD_values, 4);
	
	// 开启adc1的dma功能
	ADC_DMACmd(ADC1, ENABLE);
	
	// 开启adc
	ADC_Cmd(ADC1, ENABLE);
	
	// 校准adc
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

void ADC_GetValue(void)
{
	// 先disable
	DMA_Cmd(DMA1_Channel1, DISABLE);	
	// 设置传输计数器
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	// 再使能
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 等待传输完成
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 需要手动清0
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

void DMA_ADC_Test(void)
{
	ADC_SingleChannelInit();
	OLED_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	while (1) {
		ADC_GetValue();
	
		OLED_ShowNum(1, 10, AD_values[0], 4);
		OLED_ShowNum(2, 10, AD_values[1], 4);
		OLED_ShowNum(3, 10, AD_values[2], 4);
		OLED_ShowNum(4, 10, AD_values[3], 4);
		
		Delay_ms(100);
	}
}

