#include "MPU6050.h"

void MPU6050_Test(void)
{
	struct MPU6050_Data *data;
	
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "ID: 0x");
	OLED_ShowHexNum(1, 7, MPU6050_GetID(), 2); 
	while (1) {
		data = MPU6050_GetData();
		OLED_ShowSignedNum(2, 1, data->Accl_x, 5);
		OLED_ShowSignedNum(3, 1, data->Accl_y, 5);
		OLED_ShowSignedNum(4, 1, data->Accl_z, 5);
		OLED_ShowSignedNum(2, 8, data->Gyro_x, 5);
		OLED_ShowSignedNum(3, 8, data->Gyro_y, 5);
		OLED_ShowSignedNum(4, 8, data->Gyro_z, 5);
		Delay_ms(10);
	}
}

