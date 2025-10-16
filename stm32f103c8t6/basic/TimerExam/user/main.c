#include "stm32f10x.h"
#include "../driver/OLED.h"
#include "../driver/Timer.h"
uint32_t num = 0;

int main(void)
{
	TIM2_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "hello,world");
	OLED_ShowString(2, 1, "NUM:");
	while (1) {
		OLED_ShowNum(2, 6, num, 6);
	}
}