#include "REGX52.H"
#include "INTRINS.H"
#include "UART.h"
#include "delay.h"

extern unsigned char char_buf[100];
extern int i;

void main()
{
	UART_init();
	while (1) {
		if (P3_1 == 0) {
			Delay1ms(20);
			while (P3_1 == 0);
			Delay1ms(20);
			char_buf[i] = '\0';
			UART_send_string(char_buf);
		}
	}
}

