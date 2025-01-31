/***
 * @brief 用gpio软件模拟I2C通信
 */

#include "Delay.h"

#define I2C_PORT		GPIOB
#define SCL_PIN			GPIO_Pin_10
#define SDA_PIN			GPIO_Pin_11

void SCL_SET(uint8_t x)	 { GPIO_WriteBit(I2C_PORT, SCL_PIN, (BitAction)x);  Delay_us(10); }
void SDA_SET(uint8_t x)	 { GPIO_WriteBit(I2C_PORT, SDA_PIN, (BitAction)x);  Delay_us(10); }
uint8_t SDA_GET() { return GPIO_ReadInputDataBit(I2C_PORT, SDA_PIN); }

void I2C_SIM_Init(void)
{
	// 初始化gpio
	GPIO_InitTypeDef gpio_cfg;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_Out_OD;		// i2c必须是开漏输出
	gpio_cfg.GPIO_Pin = SCL_PIN | SDA_PIN;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_PORT, &gpio_cfg);
	GPIO_SetBits(GPIOB, SCL_PIN | SDA_PIN);		// 总线拉高，表示空闲状态
}

void I2C_SIM_Start(void)
{
	SDA_SET(1);
	SCL_SET(1);
	SDA_SET(0);
	SCL_SET(0);
}

void I2C_SIM_Stop(void)
{
	SDA_SET(0);
	SCL_SET(1);
	SDA_SET(1);
}

uint8_t I2C_SIM_RecvByte(void)
{
	uint8_t val = 0, i;
	
	SDA_SET(1);	Delay_us(10);
	for (i = 0; i < 8; i++) {
		SCL_SET(1);
		val = (val << 1) | SDA_GET();
		SCL_SET(0);
	}

	return val;
}

void I2C_SIM_SendByte(uint8_t data)
{
	uint8_t i;
	
	for (i = 0; i < 8; i++) {
		SDA_SET(data & (0x80>>i));
		SCL_SET(1);
		SCL_SET(0);
	}
}

uint8_t I2C_SIM_RecvAck(void)
{
	uint8_t ack = 0;

	SDA_SET(1); 	// 释放sda总线
	SCL_SET(1); 
	ack = SDA_GET();
	SCL_SET(0);
	
	return ack;
}

/**
 * @brief 发送ack，ack=1为非应答，0为应答
 */
void I2C_SIM_SendAck(uint8_t ack)
{
	SDA_SET(ack);
	SCL_SET(1);
	SCL_SET(0);
}


