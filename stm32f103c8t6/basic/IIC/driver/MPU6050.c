#include "stm32f10x.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDR 0xD0
#define HARD_IIC

#ifdef SOFT_IIC
int8_t MPU6050_write_reg(uint8_t reg_addr, uint8_t data)
{
	IIC_start();
	IIC_send_byte(MPU6050_ADDR);
	IIC_receive_ack();
	IIC_send_byte(reg_addr);
	IIC_receive_ack();
	IIC_send_byte(data);
	IIC_receive_ack();
	IIC_stop();
	return 0;
}

uint8_t MPU6050_read_reg(uint8_t reg_addr)
{
	/* 先写要读的寄存器地址, 再写读寄存器命令 */
	uint8_t receive_data;
	
	IIC_start();
	IIC_send_byte(MPU6050_ADDR);
	IIC_receive_ack();
	IIC_send_byte(reg_addr);
	IIC_receive_ack();

	IIC_start();
	IIC_send_byte(MPU6050_ADDR | 0x01);
	IIC_receive_ack();
	receive_data = IIC_receive_byte();
	IIC_send_ack(1);
	IIC_stop();

	return receive_data;
}

uint8_t MPU6050_get_id()
{
	return MPU6050_read_reg(MPU6050_WHO_AM_I);
}

void MPU6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}

void MPU6050_init()
{
	IIC_init();
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_write_reg(MPU6050_PWR_MGMT_1, 0x01);	//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_write_reg(MPU6050_PWR_MGMT_2, 0x00);	//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_write_reg(MPU6050_SMPLRT_DIV, 0x09);	//采样率分频寄存器，配置采样率
	MPU6050_write_reg(MPU6050_CONFIG, 0x06);		//配置寄存器，配置DLPF
	MPU6050_write_reg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_write_reg(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
}
#endif

#ifdef HARD_IIC

void MPU6050_write_reg(uint8_t reg_addr, uint8_t data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);	// 等待ev5事件

	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);	// 等待ev6事件

	I2C_SendData(I2C2, reg_addr);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);	// 等待ev8事件

	I2C_SendData(I2C2, data);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);	// 等待ev8_2事件

	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_read_reg(uint8_t reg_addr)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);	// 等待ev5事件

	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);	// 等待ev6事件

	I2C_SendData(I2C2, reg_addr);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);	// 等待ev8_2事件, 即没有新的数据要发
	
	I2C_GenerateSTART(I2C2, ENABLE);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);	// 等待ev5事件
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);	// 等待ev6事件

	// 只读一个字节, 要求不应答和stop
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);

	while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);	// 等待ev7事件, 即受到一个字节

	uint8_t data = I2C_ReceiveData(I2C2);

	return data;
}

uint8_t MPU6050_get_id()
{
	return MPU6050_read_reg(MPU6050_WHO_AM_I);
}

void MPU6050_get_data(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_read_reg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_read_reg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}

void MPU6050_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTypeDef IIC_InitStructure;
	IIC_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	IIC_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	// 作为从机相应7位地址 
	IIC_InitStructure.I2C_ClockSpeed = 50000;
	IIC_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	IIC_InitStructure.I2C_Mode = I2C_Mode_I2C;
	IIC_InitStructure.I2C_OwnAddress1 = 0xff;	// 作为从机相应7位地址实际值
	I2C_Init(I2C2, &IIC_InitStructure);

	I2C_Cmd(I2C2, ENABLE);
	
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_write_reg(MPU6050_PWR_MGMT_1, 0x01);	//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_write_reg(MPU6050_PWR_MGMT_2, 0x00);	//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_write_reg(MPU6050_SMPLRT_DIV, 0x09);	//采样率分频寄存器，配置采样率
	MPU6050_write_reg(MPU6050_CONFIG, 0x06);		//配置寄存器，配置DLPF
	MPU6050_write_reg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_write_reg(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
}
#endif
