
#include "can.h"
#include "OLED.h"
#include "key.h"
#include "stm32f10x.h"                  // Device header

uint8_t KeyNum;
uint32_t TxID = 0x555;			// device 1
// uint32_t TxID = 0x666;		// device 2
uint8_t TxLength = 4;
uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44};

uint32_t RxID;
uint8_t RxLength;
uint8_t RxData[8];

CanTxMsg tx_msgs[] = {
/*   StdId     ExtId         IDE             RTR        DLC         Data[8]          */
	{0x100, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x101, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x1FE, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x1FF, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	
	{0x200, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x201, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x2FE, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x2FF, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	
	{0x310, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x311, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x31E, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x31F, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	
	{0x320, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x321, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x32E, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
	{0x32F, 0x00000000, CAN_Id_Standard, CAN_RTR_Data,   4, {0x11, 0x22, 0x33, 0x44}},
};
int tx_msg_len = sizeof(tx_msgs) / sizeof(CanTxMsg);


void CAN_Test(void) {
	unsigned int i = 0;
	CanRxMsg rx_msg;

	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_11);
	MyCAN_Init();
	
	OLED_ShowString(1, 1, "Rx:");
	OLED_ShowString(2, 1, "RxID:");
	OLED_ShowString(3, 1, "Leng:");
	OLED_ShowString(4, 1, "Data:");

	while (1) {
		if (Is_KeyDown(GPIOB, GPIO_Pin_11)) {
			MyCAN_Transmit(&tx_msgs[i++ % tx_msg_len]);
		}
		
		// check rx data avaiable
		if (MyCAN_ReceiveFlag()) {
			MyCAN_Recieve(&rx_msg);

			if (rx_msg.IDE == CAN_Id_Standard) {
				OLED_ShowString(1, 6, "Std");
				OLED_ShowHexNum(2, 6, rx_msg.StdId, 8);
			} else if (rx_msg.IDE == CAN_Id_Extended) {
				OLED_ShowString(1, 6, "Ext");
				OLED_ShowHexNum(2, 6, rx_msg.ExtId, 8);
			}

			if (rx_msg.RTR == CAN_RTR_Data) {
				OLED_ShowString(1, 10, "Data  ");
				OLED_ShowHexNum(3, 6, rx_msg.DLC, 1);
				OLED_ShowHexNum(4, 6, rx_msg.Data[0], 2);
				OLED_ShowHexNum(4, 9, rx_msg.Data[1], 2);
				OLED_ShowHexNum(4, 12, rx_msg.Data[2], 2);
				OLED_ShowHexNum(4, 15, rx_msg.Data[3], 2);
			} else if (rx_msg.RTR == CAN_RTR_Remote) {
				OLED_ShowString(1, 10, "Remote");
				OLED_ShowHexNum(3, 6, rx_msg.DLC, 1);
				OLED_ShowHexNum(4, 6, 0x00, 2);
				OLED_ShowHexNum(4, 9, 0x00, 2);
				OLED_ShowHexNum(4, 12, 0x00, 2);
				OLED_ShowHexNum(4, 15, 0x00, 2);
			}

		}
	}
}


