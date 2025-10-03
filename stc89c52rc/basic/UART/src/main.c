#include "REGX52.H"
#include "INTRINS.H"
#include "UART.h"
#include "delay.h"

int j = 0;
extern unsigned char char_buf[100];
void main()
{
	UART_init();
	while (1) {
		if (P3_1 == 0) {
			Delay1ms(20);
			while (P3_1 == 0);
			Delay1ms(20);
			UART_send_byte(char_buf[j++]);
			UART_send_byte(char_buf[j++]);
		}
	}
}

