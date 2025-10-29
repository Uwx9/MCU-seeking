#include "stm32f10x.h"
#include "../driver/USART.h"
#include "../driver/OLED.h"
#include "stdio.h"
#include "string.h"
#include "../driver/led.h"

int main(void)
{
	
	led_init();
	MyUSART_init();
	OLED_Init();
	// usart_send_byte(0x52);
	// char arr[] = {0x11, 0x22, 0x33};
	// usart_send_char_arr(arr);
	// usart_send_num(123455, 6);
	// usart_send_string("helloworld");
	// usart_printf("hello, world%s", "!");
	
	while (1) {
		// OLED_ShowString(1, 1, "send_pkg:");
		// OLED_ShowHexNum(2, 1, TXpkg[0], 2);
		// OLED_ShowHexNum(2, 4, TXpkg[1], 2);
		// OLED_ShowHexNum(2, 7, TXpkg[2], 2);
		// OLED_ShowHexNum(2, 10, TXpkg[3], 2);
	
		// OLED_ShowString(3, 1, "receive_pkg:");
		// OLED_ShowHexNum(4, 1, RXpkg[0], 2);
		// OLED_ShowHexNum(4, 4, RXpkg[1], 2);
		// OLED_ShowHexNum(4, 7, RXpkg[2], 2);
		// OLED_ShowHexNum(4, 10, RXpkg[3], 2);
		if (get_usart_flag()) {
			OLED_ShowString(1, 1, "rx_text:");
			OLED_ShowString(2, 1, "                ");
			OLED_ShowString(2, 1, RXpkg);
			if (strcmp(RXpkg, "LED_ON") == 0) {
				led1_on();
			} else if (strcmp(RXpkg, "LED_OFF") == 0) {
				led1_off();
			}
		}
	}

}