#ifndef __MY_CAN_H
#define __MY_CAN_H

#include "stm32f10x.h"                  // Device header


void MyCAN_Init(void);
void MyCAN_Transmit(CanTxMsg *msg);
int MyCAN_ReceiveFlag(void);
void MyCAN_Recieve(CanRxMsg *msg);



#endif

