#include "stm32f10x.h"
#include "../driver/Car.h"
#include "../driver/USART.h"
#include "string.h"
#include "../driver/OLED.h"

int main(void)
{
	OLED_Init();
	car_init();
	MyUSART_init();
	while (1) {
		if (get_usart_flag()) {
			if (strcmp(RXpkg, "car_forward") == 0) {
				car_move_forward();
			} else if (strcmp(RXpkg, "car_backward") == 0) {
				car_move_backward();
			} else if (strcmp(RXpkg, "car_stop") == 0) {
				car_stop();
			} else if (strcmp(RXpkg, "car_left") == 0) {
				car_left_reversal();
			} else if (strcmp(RXpkg, "car_right") == 0) {
				car_right_reversal();
			} else if (strcmp(RXpkg, "car_speedup") == 0) {
				car_speed_up();
			} else if (strcmp(RXpkg, "car_speeddown") == 0) {
				car_speed_down();
			}
			car_show_status();
		}
	}
}