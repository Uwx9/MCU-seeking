#include "stm32f10x.h"

#define IIC_PORT GPIOB
#define SDA_PIN GPIO_Pin_11
#define SCL_PIN GPIO_Pin_10

#define IIC_W_SDA(x) GPIO_WriteBit(IIC_PORT, SDA_PIN, (BitAction)x)
#define IIC_W_SCL(x) GPIO_WriteBit(IIC_PORT, SCL_PIN, (BitAction)x)
#define IIC_R_SDA() GPIO_ReadInputDataBit(GPIOB, SDA_PIN)



void IIC_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SDA_PIN | SCL_PIN;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
	
}

void IIC_start()
{
	IIC_W_SCL(1);
	IIC_W_SDA(1);
	IIC_W_SDA(0);
	IIC_W_SCL(0);
}

void IIC_stop()
{
	IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SDA(1);
}

void IIC_send_byte(uint8_t sendbyte)
{
	for (int i = 7; i >= 0; i--) {
		IIC_W_SDA((BitAction)((sendbyte >> i) & 0x01));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
}

uint8_t IIC_receive_byte()
{
	uint8_t recbyte = 0x00;
	IIC_W_SDA(1);	// 主机释放SDA
	for (int i = 0; i < 8; i++) {
		IIC_W_SCL(1);
		if (IIC_R_SDA() == 1) {
			recbyte |= (0x80 >> i);
		}
		IIC_W_SCL(0);
	}
	return recbyte;
}

void IIC_send_ack(uint8_t ack_bit)
{
	IIC_W_SDA(ack_bit);	// 主机准备应答信号
	IIC_W_SCL(1);		// 从机读取ack
	IIC_W_SCL(0);
}

uint8_t IIC_receive_ack()
{
	uint8_t ack_bit;
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	ack_bit = IIC_R_SDA();
	IIC_W_SCL(0);
	return ack_bit;
}