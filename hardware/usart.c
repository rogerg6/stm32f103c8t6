/**
 * @brief uart1收发数据
 *
 * 硬件连接：usb转ttl模块 
 *			 vcc和3.3v短接
 *			 GND接地
 *			 RX,TX 分别接 PA9,PA10
 */
#include "usart.h"
#include "OLED.h"
#include "Delay.h"
#include "stdio.h"		// 魔术棒中需要开启use microlib

static uint8_t g_rx_data;
uint8_t g_rx_flag;

void Serial_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	// 配置gpio
	// tx
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_cfg.GPIO_Pin = GPIO_Pin_9;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
	gpio_cfg.GPIO_Pin = GPIO_Pin_10;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	// 配置uart
	USART_InitTypeDef usart_cfg;
	usart_cfg.USART_BaudRate = 9600;										// 波特率9600
	usart_cfg.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件流控
	usart_cfg.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 接收 & 发送
	usart_cfg.USART_Parity = USART_Parity_No;								// 无校验位
	usart_cfg.USART_StopBits = USART_StopBits_1;							// 1个停止位
	usart_cfg.USART_WordLength = USART_WordLength_8b;						// 8位数据
	USART_Init(USART1, &usart_cfg);
	
	// 开启rx中断，在中断handler中接受数据
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
		// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断分组
	
	NVIC_InitTypeDef nvic_cfg;
	nvic_cfg.NVIC_IRQChannel = USART1_IRQn;
	nvic_cfg.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_cfg.NVIC_IRQChannelSubPriority = 1;
	nvic_cfg.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_cfg);

	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t byte)
{
	USART_SendData(USART1, byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

uint8_t Serial_RecvByte(void)
{
	/* 轮询接收数据 */
	//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	//return (uint8_t)USART_ReceiveData(USART1);
	
	/* 返回中断中获取的rx数据 */
	return g_rx_data;
}

// rx中断处理函数
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		g_rx_data = USART_ReceiveData(USART1);
		g_rx_flag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void Serial_SendArray(uint8_t *a, uint16_t len)
{
	int i;
	for (i = 0; i < len; i++)
		Serial_SendByte(a[i]);
}
	
void Serial_SendString(char *str)
{
	while (*str != '\0')
		Serial_SendByte(*str++);
}

void Serial_SendNumber(uint32_t n)
{
	int i = 0;
	char buf[20];
	while (n) {
		buf[i++] = n % 10 + '0';
		n /= 10;
	}
	while (--i >= 0)
		Serial_SendByte(buf[i]);
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

