#include "REG52.H"
#include "INTRINS.H"
#include "IIC_EEPROM.h"
#include "delay.h"
#define nop() _nop_()

unsigned char send[2] = {0x22, 0x33};
unsigned char rece[2] = {0};
void main()
{
	if (IIC_write_str_with_addr(0xa0, 4, &send, 2) == -1) {
		P0 = 0x3c;
		Delay1ms(200);
	}
	Delay1ms(5);
	if (IIC_read_str_with_addr(0xa0, 4, &rece, 2) == -1) {
		P0 = 0x3c;
		Delay1ms(200);
	}
	P0 = rece[0];
	Delay1ms(10000);
	P0 = rece[1];
	while (1);
}