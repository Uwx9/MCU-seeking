#include "stm32f10x.h"
#include "SPI.h"
#include "W25Q64_INS.h"

static void W25Q64_write_enable()
{
	spi_start();
	spi_swap_byte(W25Q64_WRITE_ENABLE);
	spi_stop();
}

static void W25Q64_wait_busy()
{
	spi_start();
	spi_swap_byte(W25Q64_READ_STATUS_REGISTER_1);
	while (spi_swap_byte(W25Q64_READ_STATUS_REGISTER_1) & 0x01 == 0x01);	// busy为1时一直等
	spi_stop();
}

void W25Q64_init()
{
	spi_init();
}

void W25Q64_read_id(uint8_t* MID, uint16_t* DID)
{
	spi_start();
	spi_swap_byte(0x9f);
	*MID = spi_swap_byte(0xff);
	*DID = spi_swap_byte(0xff);
	*DID <<= 8;
	*DID |= spi_swap_byte(0xff);
	spi_stop();
}

void W25Q64_page_program(uint32_t addr, uint8_t* data_arry, uint16_t count)
{
	W25Q64_write_enable();	// 写使能, stop后会失能
	spi_start();
	spi_start();
	spi_swap_byte(W25Q64_PAGE_PROGRAM);
	spi_swap_byte((uint8_t)(addr >> 16));
	spi_swap_byte((uint8_t)(addr >> 8));
	spi_swap_byte((uint8_t)addr);
	for (int i = 0; i < count; i++) {
		spi_swap_byte(data_arry[i]);
	}
	spi_stop();
	W25Q64_wait_busy();
}

void W25Q64_sector_erase(uint32_t addr)
{
	W25Q64_write_enable();	// 写使能
	spi_start();
	spi_swap_byte(W25Q64_SECTOR_ERASE_4KB);
	spi_swap_byte((uint8_t)(addr >> 16));
	spi_swap_byte((uint8_t)(addr >> 8));
	spi_swap_byte((uint8_t)addr);
	spi_stop();
	W25Q64_wait_busy();
}

void W25Q64_read_data(uint32_t addr, uint8_t* data_arry, uint32_t count)
{
	spi_start();
	spi_swap_byte(W25Q64_READ_DATA);
	spi_swap_byte((uint8_t)(addr >> 16));
	spi_swap_byte((uint8_t)(addr >> 8));
	spi_swap_byte((uint8_t)addr);
	for (int i = 0; i < count; i++) {
		data_arry[i] = spi_swap_byte(W25Q64_DUMMY_BYTE);
	}
	spi_stop();
}