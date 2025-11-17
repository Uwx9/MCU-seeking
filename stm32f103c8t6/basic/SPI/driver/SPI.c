#include "stm32f10x.h"

#define HARD_SPI 

static void spi_w_ss(uint8_t bit_value)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bit_value);
}

void spi_start()
{
	spi_w_ss(0);
}

void spi_stop()
{
	spi_w_ss(1);
}

#ifdef SORT_SPI
static void spi_w_sck(uint8_t bit_value)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)bit_value);
}

static void spi_w_MOSI(uint8_t bit_value)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)bit_value);
}

static uint8_t spi_r_MISO()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

/* PA6为MIDO,上拉输入, PA4,5,7推挽输出 */
void spi_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 设置默认电平 */
	spi_w_ss(1);
	spi_w_sck(0);
}

uint8_t spi_swap_byte(uint8_t sendbyte)
{
	uint8_t ret = 0;
	
	for (int i = 7; i >= 0; i--) {
		ret <<= 1;
		spi_w_MOSI((sendbyte >> i) & 0x01);
		spi_w_sck(1);
		if (spi_r_MISO() == 1)
			ret	|= 0x01;
		spi_w_sck(0);
	}
	return ret;
}

#endif

#ifdef HARD_SPI
void spi_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	// 模式0
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	// 模式0
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	// 意思是软件模拟ss
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);

	spi_w_ss(1);
}

uint8_t spi_swap_byte(uint8_t sendbyte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	SPI_I2S_SendData(SPI1, sendbyte);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_I2S_ReceiveData(SPI1);
}

#endif