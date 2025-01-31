/***
 * @brief 软件模拟/硬件操作MPU6050 6轴姿态传感器
 *
 * 硬件连接：MPU6050
 *			 VCC, GND分别连接3.3v, GND
 *			 SCL, SDA分别连接PB10, PB11
 */

#include "MPU6050.h"


#define USE_I2C_SIMULATION 0		// 是否使用软件模拟i2c

struct MPU6050_Data g_MPU6050_data;

#if USE_I2C_SIMULATION

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

#else

/**
 * @brief 防止总线故障导致卡住，加入超时机制
 */
static void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t timeout = 0x1000;
	
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS){
		if (timeout-- == 0) {
			// TODO: 超时处理
			break;
		}
	}
}

/**
 * @brief 写MPU6050的reg_addr寄存器写入数据data
 *		  成功返回0，否则返回1
 */
static int MPU6050_WriteReg(uint8_t reg_addr, uint8_t data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);	// ev5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	// ev6
	
	I2C_SendData(I2C2, reg_addr);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);	// ev8
	
	I2C_SendData(I2C2, data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);		// ev8_2

	I2C_GenerateSTOP(I2C2, ENABLE);
	return 0;
}

/**
 * @brief 读MPU6050的reg_addr寄存器数据到data
 *		  成功返回0，否则返回1
 */
static int MPU6050_ReadReg(uint8_t reg_addr, uint8_t *data)
{
	// write
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);	// ev5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	// ev6
	
	I2C_SendData(I2C2, reg_addr);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);	// ev8_2
	
	// read
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);	// ev5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);	// ev6
	
	// 在接收最后一个byte数据前，清除响应并发出stop信号
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);

	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);	// ev7
	*data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return 0;
}

void MPU6050_Init(void)
{
	// 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 配置GPIO
	GPIO_InitTypeDef gpio_cfg;
	gpio_cfg.GPIO_Mode = GPIO_Mode_AF_OD;		// i2c必须是开漏输出
	gpio_cfg.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio_cfg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_cfg);

	// 配置I2C
	I2C_InitTypeDef i2c_cfg;
	i2c_cfg.I2C_Mode = I2C_Mode_I2C;
	i2c_cfg.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		// 7位地址模式
	i2c_cfg.I2C_Ack = I2C_Ack_Enable;									// 开启回复ack
	i2c_cfg.I2C_ClockSpeed = 50000;										// 50KHz
	i2c_cfg.I2C_DutyCycle = I2C_DutyCycle_2;		// 0-100KHz默认1:1, 100-400KHz可设置为16:9或2:1
	i2c_cfg.I2C_OwnAddress1 = 0x00;					// 只要和从机地址不重复即可
	I2C_Init(I2C2, &i2c_cfg);
	
	I2C_Cmd(I2C2, ENABLE);
	
	// 初始化MPU6050
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//配置陀螺仪寄存器，配置为满量程2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//配置加速度寄存器，配置为满量程±16g
}


#endif  // USE_I2C_SIMULATION


uint8_t MPU6050_GetID(void)
{
	uint8_t data;
	MPU6050_ReadReg(MPU6050_WHO_AM_I, &data);
	return data;
}

struct MPU6050_Data *MPU6050_GetData(void)
{
	uint8_t dataH, dataL;
	
	// accl x
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L, &dataL);
	g_MPU6050_data.Accl_x = (dataH << 8) | dataL;
	// accl y
	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L, &dataL);
	g_MPU6050_data.Accl_y = (dataH << 8) | dataL;
	// accl z
	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L, &dataL);
	g_MPU6050_data.Accl_z = (dataH << 8) | dataL;
	// gyro x
	MPU6050_ReadReg(MPU6050_GYRO_XOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_XOUT_L, &dataL);
	g_MPU6050_data.Gyro_x = (dataH << 8) | dataL;
	// gyro y
	MPU6050_ReadReg(MPU6050_GYRO_YOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_YOUT_L, &dataL);
	g_MPU6050_data.Gyro_y = (dataH << 8) | dataL;
	// gyro z
	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H, &dataH);
	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L, &dataL);
	g_MPU6050_data.Gyro_z = (dataH << 8) | dataL;
	
	return &g_MPU6050_data;
}



