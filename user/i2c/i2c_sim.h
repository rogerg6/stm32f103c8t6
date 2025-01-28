#ifndef __I2C_SIMULATION_H
#define __I2C_SIMULATION_H

#include "stm32f10x.h"                  // Device header


void I2C_SIM_Init(void);
void I2C_SIM_Start(void);
void I2C_SIM_Stop(void);
uint8_t I2C_SIM_RecvByte(void);
void I2C_SIM_SendByte(uint8_t data);
uint8_t I2C_SIM_RecvAck(void);
void I2C_SIM_SendAck(uint8_t ack);




#endif

