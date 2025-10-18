// /**
//  * @file main.c
//  * @author Unique9
//  * @brief PA6,TIM3输出PWM波形, 由PA0口TIM2捕获并分析
//  * @version 0.1
//  * @date 2025-10-18
//  * 
//  * @copyright Copyright (c) 2025
//  * 
//  */

// #include "stm32f10x.h"
// #include "../driver/OLED.h"
// #include "../system/delay.h"
// #include "../driver/IC.h"
// #include "../driver/PWM.h"


// int main(void)
// {
// 	OLED_Init();
// 	IC_init();
// 	PWMTIM3_init();
// 	OLED_ShowString(1, 1, "freq:000000hz");
// 	PWNTIM3_set_psc(72 - 1);
// 	PWMTIM3_set_compare(30);
// 	OLED_ShowString(2, 1, "duty:000%");
// 	while (1) {
// 		OLED_ShowNum(1, 6, IC_get_freq(), 6);
// 		OLED_ShowNum(2, 6, IC_get_duty(), 3);
// 	}
// }

/**
 * @brief PWM电机驱动
 * 
 */
#include "stm32f10x.h"
#include "../driver/PWM.h"
#include "../driver/Motor.h"
#include "../driver/IC.h"
#include "../driver/OLED.h"

int main()
{
	motor_init();
	motor_set_speed(50);
	OLED_Init();
	IC_init();
	OLED_ShowString(1, 1, "freq:000000hz");
	OLED_ShowString(2, 1, "duty:000%");
	while (1) {
		OLED_ShowNum(1, 6, IC_get_freq(), 6);
		OLED_ShowNum(2, 6, IC_get_duty(), 3);
	}

	
}