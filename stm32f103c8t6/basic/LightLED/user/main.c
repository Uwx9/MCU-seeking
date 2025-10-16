#include "stm32f10x.h"
#include "../system/delay.h"
#include "../driver/led.h"
#include "../driver/key.h"

int main(void)
{
	led_init();
	key_init();
	while (1) {
		uint8_t key = get_keynum();
		if (key == 0) led0_turn();
		else if (key == 1) led1_turn();
	}
}