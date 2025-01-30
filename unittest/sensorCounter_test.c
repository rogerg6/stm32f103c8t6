#include "sensorCounter.h"
#include "OLED.h"
#include "Delay.h"


/**
 * 对射式红外传感器计次 测试
 * 遮挡红外传感器，OLED上显示遮挡的次数。
 *
 */
void SensorCounter_Test(void)
{
	OLED_Init();
	SensorCounter_Init();
	
	OLED_ShowString(1, 1, "cnt = ");
	
	while (1) {
		OLED_ShowNum(1, 7, SensorCounter_GetCount(), 5);
	}
}


/**
 * 停机模式(低功耗)下测试：对射式红外传感器计次。
 * 
 * 无遮挡：系统停机；有遮挡：系统被唤醒后处理中断，并闪烁Running
 */
void SensorCounter_StopMode_Test(void)
{
	OLED_Init();
	SensorCounter_Init();
	
	OLED_ShowString(1, 1, "cnt = ");
	
	while (1) {
		OLED_ShowNum(1, 7, SensorCounter_GetCount(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		/**
		 * 停机模式：cpu时钟停止
		 * PWR_Regulator_ON: 配置LPDS = 0，电压调节器开启(意味着cpu外设有点，数据不会丢失)
		 * PWR_STOPEntry_WFI: 调用__WFI()进入休眠
		**/
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
		SystemInit();	// 唤醒后系统时钟会自动切换成HSI=8M，所以需要执行SystemInit初始化系统时钟为72M
	}
}

