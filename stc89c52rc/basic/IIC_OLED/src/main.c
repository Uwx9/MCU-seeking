#include "REGX52.H"
#include "INTRINS.H"
#include "IIC.h"
#include "UART.h"
#include "stdint.h"

extern uint8_t IIC_dev_addr;
void main()
{
	UART_init();
	IIC_dev_scan();
	UART_send_byte(IIC_dev_addr);
	while (1);

}