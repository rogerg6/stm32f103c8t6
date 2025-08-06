#include "can.h"

void MyCAN_Init(void) {
	// gpio init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_cfg.GPIO_Pin = GPIO_Pin_12;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	gpio_cfg.GPIO_Mode = GPIO_Mode_IPU;
	gpio_cfg.GPIO_Pin = GPIO_Pin_11;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_cfg);
	
	// can init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN_InitTypeDef can_cfg;
	can_cfg.CAN_Mode = CAN_Mode_LoopBack;		// 回环模式
	can_cfg.CAN_Prescaler = 48;					//波特率 = 36M / 48 / (1 + 2 + 3) = 125K
	can_cfg.CAN_BS1 = CAN_BS1_2tq;
	can_cfg.CAN_BS2 = CAN_BS2_3tq;
	can_cfg.CAN_SJW = CAN_SJW_2tq;
	can_cfg.CAN_NART = DISABLE;
	can_cfg.CAN_TXFP = DISABLE;
	can_cfg.CAN_RFLM = DISABLE;
	can_cfg.CAN_AWUM = DISABLE;
	can_cfg.CAN_TTCM = DISABLE;
	can_cfg.CAN_ABOM = DISABLE;
	CAN_Init(CAN1, &can_cfg);
	
	// can filter init
	// 列表模式，只接受id为0x345遥控帧, 0x234,0x456的数据帧
	CAN_FilterInitTypeDef can_filter_cfg;
	can_filter_cfg.CAN_FilterNumber = 0;
	can_filter_cfg.CAN_FilterIdHigh = 0x234 << 5;
	can_filter_cfg.CAN_FilterIdLow = 0x345 << 5 | 0x10;
	can_filter_cfg.CAN_FilterMaskIdHigh = 0x456 << 5;
	can_filter_cfg.CAN_FilterMaskIdLow = 0x000 << 5;
	can_filter_cfg.CAN_FilterScale = CAN_FilterScale_16bit;
	can_filter_cfg.CAN_FilterMode = CAN_FilterMode_IdList;
	can_filter_cfg.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	can_filter_cfg.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter_cfg);
}

void MyCAN_Transmit(CanTxMsg *msg) {
	uint8_t tx_mailbox;
	uint32_t Timeout = 0;
	
	tx_mailbox = CAN_Transmit(CAN1, msg);
	while (CAN_TransmitStatus(CAN1, tx_mailbox) != CAN_TxStatus_Ok) {
		Timeout++;
		if (Timeout > 100000)
			break;
	}
}

int MyCAN_ReceiveFlag(void) {
	return CAN_MessagePending(CAN1, CAN_FIFO0) > 0;
}

void MyCAN_Recieve(CanRxMsg *msg) {
	CAN_Receive(CAN1, CAN_FIFO0, msg);
}

