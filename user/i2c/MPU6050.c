#include "MPU6050.h"
#include "OLED.h"
#include "Delay.h"


#define USE_I2C_SIMULATION 1		// 是否使用软件模拟i2c

struct MPU6050_Data g_MPU6050_data;

#ifdef USE_I2C_SIMULATION

#include "i2c_sim.h"

/**
 * @brief 写MPU6050的reg_addr寄存器写入数据data
 *		  成功返回0，否则返回1
 */
int MPU6050_WriteReg(uint8_t reg_addr, uint8_t data)
{
	I2C_SIM_Start();
	I2C_SIM_SendByte(MPU6050_ADDR_W);
	if (I2C_SIM_RecvAck())
		return -1;
	
	I2C_SIM_SendByte(reg_addr);
	if (I2C_SIM_RecvAck())
		return -2;

	I2C_SIM_SendByte(data);
	if (I2C_SIM_RecvAck())
		return -3;
	
	I2C_SIM_Stop();

	return 0;
}

/**
 * @brief 读MPU6050的reg_addr寄存器数据到data
 *		  成功返回0，否则返回1
 */
int MPU6050_ReadReg(uint8_t reg_addr, uint8_t *data)
{
	I2C_SIM_Start();
	I2C_SIM_SendByte(MPU6050_ADDR_W);
	if (I2C_SIM_RecvAck())
		return -1;
	
	I2C_SIM_SendByte(reg_addr);
	if (I2C_SIM_RecvAck())
		return -2;

	I2C_SIM_Start();
	I2C_SIM_SendByte(MPU6050_ADDR_R);
	if (I2C_SIM_RecvAck())
		return -3;
	
	*data = I2C_SIM_RecvByte();
	I2C_SIM_SendAck(1);
	I2C_SIM_Stop();
	
	return 0;
}

void MPU6050_Init(void)
{
	I2C_SIM_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//配置陀螺仪寄存器，配置为满量程2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);			//配置加速度寄存器，配置为满量程±16g
}

uint8_t MPU6050_GetID(void)
{
	uint8_t data;
	MPU6050_ReadReg(MPU6050_WHO_AM_I, &data);
	return data;
}

void MPU6050_GetData(void)
{
	uint8_t dataH, dataL;
	
	// accl x
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L, &dataL);
	g_MPU6050_data.Accl_x = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L, &dataL);
	g_MPU6050_data.Accl_y = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L, &dataL);
	g_MPU6050_data.Accl_z = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L, &dataL);
	g_MPU6050_data.Accl_x = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_GYRO_XOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_XOUT_L, &dataL);
	g_MPU6050_data.Gyro_x = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_GYRO_YOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_YOUT_L, &dataL);
	g_MPU6050_data.Gyro_y = (dataH << 8) | dataL;

	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L, &dataL);
	g_MPU6050_data.Gyro_z = (dataH << 8) | dataL;
}


void MPU6050_Test(void)
{
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "ID: 0x");
	OLED_ShowHexNum(1, 7, MPU6050_GetID(), 2); 
	while (1) {
		MPU6050_GetData();
		OLED_ShowSignedNum(2, 1, g_MPU6050_data.Accl_x, 5);
		OLED_ShowSignedNum(3, 1, g_MPU6050_data.Accl_y, 5);
		OLED_ShowSignedNum(4, 1, g_MPU6050_data.Accl_z, 5);
		OLED_ShowSignedNum(2, 8, g_MPU6050_data.Gyro_x, 5);
		OLED_ShowSignedNum(3, 8, g_MPU6050_data.Gyro_y, 5);
		OLED_ShowSignedNum(4, 8, g_MPU6050_data.Gyro_z, 5);
		Delay_ms(10);
	}
}

#else

#endif  // USE_I2C_SIMULATION

