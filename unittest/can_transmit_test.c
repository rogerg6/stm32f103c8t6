
#include "can.h"
#include "OLED.h"
#include "key.h"
#include "timer.h"
#include "stm32f10x.h"                  // Device header

uint8_t TimingFlag = 0;

CanTxMsg TxMsg_Timing = {
	.StdId = 0x100,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 4,
	.Data = {0x11, 0x22, 0x33, 0x44}
};

CanTxMsg TxMsg_Trigger = {
	.StdId = 0x200,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 4,
	.Data = {0x11, 0x22, 0x33, 0x44}
};

CanTxMsg TxMsg_Request = {
	.StdId = 0x300,
	.ExtId = 0x00000000,
	.IDE = CAN_Id_Standard,
	.RTR = CAN_RTR_Data,
	.DLC = 4,
	.Data = {0x11, 0x22, 0x33, 0x44}
};


/**
 * 一台机器发送
 */
void CAN_Transmit_Test(void) {

	OLED_Init();
	KEY_INIT(GPIOB, GPIO_Pin_11);
	MyCAN_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Tx:");
	OLED_ShowString(2, 1, "Tim:");
	OLED_ShowString(3, 1, "Tri:");
	OLED_ShowString(4, 1, "Req:");

	while (1) {
		// 定时发送
		if (TimingFlag) {
			TimingFlag = 0;

			TxMsg_Timing.Data[0]++;
			TxMsg_Timing.Data[1]++;
			TxMsg_Timing.Data[2]++;
			TxMsg_Timing.Data[3]++;
			
			MyCAN_Transmit(&TxMsg_Timing);

			OLED_ShowHexNum(2, 5, TxMsg_Timing.Data[0], 2);
			OLED_ShowHexNum(2, 8, TxMsg_Timing.Data[1], 2);
			OLED_ShowHexNum(2, 11, TxMsg_Timing.Data[2], 2);
			OLED_ShowHexNum(2, 14, TxMsg_Timing.Data[3], 2);
		}

		// 按键触发发送
		if (Is_KeyDown(GPIOB, GPIO_Pin_11)) {
			TxMsg_Trigger.Data[0]++;
			TxMsg_Trigger.Data[1]++;
			TxMsg_Trigger.Data[2]++;
			TxMsg_Trigger.Data[3]++;
			
			MyCAN_Transmit(&TxMsg_Trigger);

			OLED_ShowHexNum(3, 5, TxMsg_Trigger.Data[0], 2);
			OLED_ShowHexNum(3, 8, TxMsg_Trigger.Data[1], 2);
			OLED_ShowHexNum(3, 11, TxMsg_Trigger.Data[2], 2);
			OLED_ShowHexNum(3, 14, TxMsg_Trigger.Data[3], 2);
		}

		// 收到遥控帧 or 数据帧请求，发送
		if (rx_flag) {
			rx_flag = 0;
			if ((rx_msg.IDE == CAN_Id_Standard && rx_msg.RTR == CAN_RTR_Data && rx_msg.StdId == 0x3ff) ||
				(rx_msg.IDE == CAN_Id_Standard && rx_msg.RTR == CAN_RTR_Remote && rx_msg.StdId == 0x300)) {

				TxMsg_Request.Data[0]++;
				TxMsg_Request.Data[1]++;
				TxMsg_Request.Data[2]++;
				TxMsg_Request.Data[3]++;

				MyCAN_Transmit(&TxMsg_Request);
	
				OLED_ShowHexNum(4, 5, TxMsg_Request.Data[0], 2);
				OLED_ShowHexNum(4, 8, TxMsg_Request.Data[1], 2);
				OLED_ShowHexNum(4, 11, TxMsg_Request.Data[2], 2);
				OLED_ShowHexNum(4, 14, TxMsg_Request.Data[3], 2);
			}
		}
		
	}
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
		TimingFlag = 1;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

