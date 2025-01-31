#ifndef __MY_UART_H
#define __MY_UART_H

#include "stm32f10x.h"                  // Device header
extern uint8_t g_rx_flag;

void Serial_Init(void);
void Serial_SendByte(uint8_t byte);
uint8_t Serial_RecvByte(void);
void Serial_SendArray(uint8_t *a, uint16_t len);
void Serial_SendString(char *str);
void Serial_SendNumber(uint32_t n);






#endif

