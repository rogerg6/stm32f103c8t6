
#include "can.h"
#include "OLED.h"
#include "key.h"
#include "stm32f10x.h"                  // Device header


CanTxMsg TxMsg_Request_Remote = {
	.StdId = 0x300,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Remote,
	.DLC = 0,
	.Data = {0x00}
};

CanTxMsg TxMsg_Request_Data = {
	.StdId = 0x3FF,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 0,
	.Data = {0x00}
};



/**
 * 一台机器接收
 */
void CAN_Recieve_Test(void) {

	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_11 | GPIO_Pin_1);
	MyCAN_Init();
	
	OLED_ShowString(1, 1, "Rx:");
	OLED_ShowString(2, 1, "Tim:");
	OLED_ShowString(3, 1, "Tri:");
	OLED_ShowString(4, 1, "Req:");

	while (1) {
		if (Is_KeyDown(GPIOB, GPIO_Pin_11)) {
			MyCAN_Transmit(&TxMsg_Request_Remote);
		}
		if (Is_KeyDown(GPIOB, GPIO_Pin_1)) {
			MyCAN_Transmit(&TxMsg_Request_Data);
		}
		if (rx_flag) {
			rx_flag = 0;
			if (rx_msg.IDE == CAN_Id_Standard) {
				if (rx_msg.StdId == 0x100) {
					OLED_ShowHexNum(2, 6, rx_msg.Data[0], 2);
					OLED_ShowHexNum(2, 9, rx_msg.Data[1], 2);
					OLED_ShowHexNum(2, 12, rx_msg.Data[2], 2);
					OLED_ShowHexNum(2, 15, rx_msg.Data[3], 2);
				}
				else if (rx_msg.StdId == 0x200) {
					OLED_ShowHexNum(3, 6, rx_msg.Data[0], 2);
					OLED_ShowHexNum(3, 9, rx_msg.Data[1], 2);
					OLED_ShowHexNum(3, 12, rx_msg.Data[2], 2);
					OLED_ShowHexNum(3, 15, rx_msg.Data[3], 2);
				}
				else if (rx_msg.StdId == 0x300) {
					OLED_ShowHexNum(4, 6, rx_msg.Data[0], 2);
					OLED_ShowHexNum(4, 9, rx_msg.Data[1], 2);
					OLED_ShowHexNum(4, 12, rx_msg.Data[2], 2);
					OLED_ShowHexNum(4, 15, rx_msg.Data[3], 2);
				}
			}

		}
	}
}

