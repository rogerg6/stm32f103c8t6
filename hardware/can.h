#ifndef __MY_CAN_H
#define __MY_CAN_H

#include "stm32f10x.h"                  // Device header


void MyCAN_Init(void);
void MyCAN_Transmit(uint32_t ID, uint8_t data_len, uint8_t *data);
int MyCAN_ReceiveFlag(void);
void MyCAN_Recieve(uint32_t *ID, uint8_t *data_len, uint8_t *data);



#endif

