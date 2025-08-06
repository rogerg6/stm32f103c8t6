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
	// 32位屏蔽模式，只接受id为0x12345600 ~ 0x123456ff扩展格式的数据帧
	CAN_FilterInitTypeDef can_filter_cfg;
	can_filter_cfg.CAN_FilterNumber = 0;

	uint32_t id1 = (0x12345600 << 3) | 0x4;
	can_filter_cfg.CAN_FilterIdHigh = id1 >> 16;
	can_filter_cfg.CAN_FilterIdLow = id1;

	uint32_t mask = (0x1fffff00 << 3) | 0x4;
	can_filter_cfg.CAN_FilterMaskIdHigh = mask >> 16;
	can_filter_cfg.CAN_FilterMaskIdLow = mask;
	can_filter_cfg.CAN_FilterScale = CAN_FilterScale_32bit;
	can_filter_cfg.CAN_FilterMode = CAN_FilterMode_IdMask;
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

