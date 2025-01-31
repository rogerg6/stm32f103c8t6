/**
 * @brief uart1收发数据
 *
 * 硬件连接：usb转ttl模块 vcc和3.3v短接
 *			 RX,TX 分别接 PA9,PA10
 */
#include <stdio.h>
#include "usart.h"
#include "OLED.h"
#include "Delay.h"

// 串口测试
void Serial_Test(void)
{
	uint8_t val;
	OLED_Init();
	OLED_ShowString(1, 1, "recv: 0x");
	
	Serial_Init();
	
	Serial_SendNumber(10086);
	Serial_SendByte('\n');
	Serial_SendByte(0x41);
	Serial_SendByte(0x42);
	Serial_SendByte(0x43);
	Serial_SendByte(0x44);
	Serial_SendString("\nUSaRT test\n");
	printf("It's stm32f103c8t6 serial demo\n");
	
	while (1) {
		if (g_rx_flag) {
			val = Serial_RecvByte();
			OLED_ShowHexNum(1, 9, val, 2);
			Serial_SendByte(val);
			g_rx_flag = 0;
		}
	}
}

/***
 * @brief 串口低功耗测试
 *        当串口未收到数据时，系统进入睡眠模式；当串口
 *		  由数据来会触发中断，唤醒系统，接受数据同时Running闪烁
 * 
**/
void Serial_LowPower_Test(void)
{	
	OLED_Init();
	OLED_ShowString(1, 1, "recv: 0x");
	
	Serial_Init();
	
	Serial_SendNumber(10086);
	Serial_SendByte('\n');
	Serial_SendByte(0x41);
	Serial_SendByte(0x42);
	Serial_SendByte(0x43);
	Serial_SendByte(0x44);
	Serial_SendString("\nUSaRT test\n");
	printf("It's stm32f103c8t6 serial demo\n");
	
	while (1) {
		if (g_rx_flag) {
			OLED_ShowHexNum(1, 9, Serial_RecvByte(), 2);
			g_rx_flag = 0;
		}
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		// 进入睡眠模式。可由中断唤醒
		__WFI();
	}
}

