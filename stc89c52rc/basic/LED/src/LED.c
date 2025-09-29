#include <reg52.h>
#include "delay.h"
#include "port.h"

/* LED 呼吸灯 */
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



