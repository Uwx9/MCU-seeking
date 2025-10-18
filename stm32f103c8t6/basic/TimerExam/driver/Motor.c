#include "PWM.h"
#include "stm32f10x.h"

void motor_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initstructure);
	
	PWMTIM3_init();
}

void motor_set_speed(int8_t speed)
{
	if (speed < -100 || speed > 100) return;
	if (speed >= 0) {
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		PWMTIM3_set_compare(1, speed);
	} else {
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		PWMTIM3_set_compare(1, -speed);
	}
}