/**
 * @file Car.c
 * @author your name (you@domain.com)
 * @brief 这里速度不用关心方向
 * @brief 用A2和A3控制电机1方向, 用A4和A5控制电机2方向
 * @version 0.1
 * @date 2025-10-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f10x.h"
#include "Car.h"
#include "PWM.h"
#include "stdbool.h"
#include "OLED.h"

enum CAR_SPEED CarSpeed;	// 小车运行时的速度
enum CAR_SPEED ReadySpeed;	// 小车停止时为下次运行时预设的速度, 跑动后CarSpeed = ReadySpeed

bool TurnLeft = false;
bool TurnRight = false;
uint8_t LeftSpeed = 0;
uint8_t RightSpeed = 0;

/**
 * @brief 设置单个电机速度
 * 
 * @param MotorId 电机ID, 接TIM3_CH1的电机ID为1, 接TIM3_CH2的电机的ID为2
 * @param MotorSpeed 
 */
static void motor_set_speed(uint8_t MotorId, uint8_t MotorSpeed)
{
	PWMTIM3_set_compare(MotorId, MotorSpeed);
}

/* 重置小车速度, 意思是左右轮速度一样 */
static void car_reset_speed(enum CAR_SPEED _CarSpeed)
{
	uint8_t LRMotorSpeed;

	if (_CarSpeed == CAR_SPEED_0) {
		LRMotorSpeed = 0;
	}
	else if (_CarSpeed == CAR_SPEED_1)
		LRMotorSpeed = 60;
	else if (_CarSpeed == CAR_SPEED_2)
		LRMotorSpeed = 80;
	else if (_CarSpeed == CAR_SPEED_3)
		LRMotorSpeed = 100;

	motor_set_speed(1, LRMotorSpeed);
	motor_set_speed(2, LRMotorSpeed);
	
	CarSpeed = _CarSpeed;
	LeftSpeed = LRMotorSpeed;
	RightSpeed = LRMotorSpeed;
}

/* 小车初始化, 初始化PWM然后让车速为0 */
void car_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_initstructure;
	GPIO_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initstructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initstructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);

	PWMTIM3_init();
	car_reset_speed(CAR_SPEED_0);
	ReadySpeed = CAR_SPEED_0;
}

/* 停车 */
void car_stop()
{
	car_reset_speed(CAR_SPEED_0);
	CarSpeed = CAR_SPEED_0;
	ReadySpeed = CAR_SPEED_0;
}

/* 前进 */
void car_move_forward()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	
	if (ReadySpeed == CAR_SPEED_0) {
		ReadySpeed = CAR_SPEED_1;
	}
	car_reset_speed(ReadySpeed);
}

/* 后退 */
void car_move_backward()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	if (ReadySpeed == CAR_SPEED_0) {
		ReadySpeed = CAR_SPEED_1;
	}
	car_reset_speed(ReadySpeed);
}

/* 反转左轮状态 */
void car_left_reversal()
{
	if (CarSpeed != CAR_SPEED_0) {
		if (!TurnLeft) {
			LeftSpeed = LeftSpeed - 10;
			motor_set_speed(1, LeftSpeed);
			TurnLeft = true;
		} else {
			LeftSpeed = LeftSpeed + 10;
			motor_set_speed(1, LeftSpeed);
			TurnLeft = false;
		}
	}
}

/* 反转右轮状态 */
void car_right_reversal()
{
	if (CarSpeed != CAR_SPEED_0) {
		if (!TurnRight) {
			RightSpeed = RightSpeed - 10;
			motor_set_speed(1, RightSpeed);
			TurnRight = true;
		} else {
			RightSpeed = RightSpeed + 10;
			motor_set_speed(1, RightSpeed);
			TurnRight = false;
		}
	}
}

/* 如果车车有速度则速度加一级, stop状态就改变预备速度, 下次启动用这个速度 */
void car_speed_up()
{
	if (CarSpeed == CAR_SPEED_0) {
		if (ReadySpeed == CAR_SPEED_0)
			ReadySpeed = CAR_SPEED_1;
		else if (ReadySpeed == CAR_SPEED_1)
			ReadySpeed = CAR_SPEED_2;
		else if (ReadySpeed == CAR_SPEED_2)
			ReadySpeed = CAR_SPEED_3;
	} else if (CarSpeed == CAR_SPEED_1) {
		car_reset_speed(CAR_SPEED_2);
	} else if (CarSpeed == CAR_SPEED_2) {
		car_reset_speed(CAR_SPEED_3);
	}
}

/* 如果车车有速度则速度减一级, stop状态就改变预备速度, 下次启动用这个速度 */
void car_speed_down()
{
	if (CarSpeed == CAR_SPEED_0) {
		if (ReadySpeed == CAR_SPEED_3)
			ReadySpeed = CAR_SPEED_2;
		else if (ReadySpeed == CAR_SPEED_2)
			ReadySpeed = CAR_SPEED_1;
		else if (ReadySpeed == CAR_SPEED_1)
			ReadySpeed = CAR_SPEED_0;
	} else if (CarSpeed == CAR_SPEED_3) {
		car_reset_speed(CAR_SPEED_2);
	} else if (CarSpeed == CAR_SPEED_2) {
		car_reset_speed(CAR_SPEED_1);
	} else if (CarSpeed == CAR_SPEED_1) {
		car_reset_speed(CAR_SPEED_0);
	}
}

/* 显示car状态 */
void car_show_status()
{
	OLED_ShowString(1, 1, "speed:");
	OLED_ShowNum(1, 8, CarSpeed, 3);
	OLED_ShowString(2, 1, "ReadySpeed:");
	OLED_ShowNum(2, 13, ReadySpeed, 3);
}