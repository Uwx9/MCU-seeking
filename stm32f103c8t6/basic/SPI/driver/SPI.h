#ifndef __SOFT_SPI_H__
#define __SOFT_SPI_H__

void spi_init();
void spi_start();
void spi_stop();
uint8_t spi_swap_byte(uint8_t sendbyte);

#endif