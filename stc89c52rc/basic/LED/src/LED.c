#include <reg52.h>
#include "delay.h"

void P0_set1()
{
	P00 = P01 = P02 = P03 = P04 = P05 = P06 = P07 = 1;
}

void P0_set0()
{
	P00 = P01 = P02 = P03 = P04 = P05 = P06 = P07 = 0;
}

void LED_breath_by_P0(void)
{
	int T = 0, Toff = 500;
	while (1) {
		for (T = 0; T < 500; T++) {
			P0_set1();
			delay(T);
			P0_set0();
			delay(500 - T);
		}

		for (Toff = 500; Toff > 0; Toff--) {
			P0_set1();
			delay(Toff);
			P0_set0();
			delay(500 - Toff);
		}

		delay_100ms();
	}
}