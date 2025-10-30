/**
 * @file Car.c
 * @author your name (you@domain.com)
 * @brief 这里速度不用关心方向
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

enum CAR_SPEED CarSpeed;

bool TurnLeft = false;
bool TurnRight = false;
uint8_t LeftSpeed = 0;	// from -100 ~ 100
uint8_t RightSpeed = 0;

/* 小车初始化, 初始化PWM然后让车速为0 */
void car_init()
{
	PWMTIM3_init();
	car_reset_speed(CAR_SPEED_0);
}

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
void car_reset_speed(enum CAR_SPEED _CarSpeed)
{
	uint8_t LRMotorSpeed;

	if (_CarSpeed == CAR_SPEED_0)
		LRMotorSpeed = 0;
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

/* 左转, 让左轮速度-10 */
void car_turnleft()
{
	if (!TurnLeft) {
		LeftSpeed = LeftSpeed - 10;
		motor_set_speed(1, LeftSpeed);
		TurnLeft = true;
	}
}

/* 停止左转, 让左轮速度+10 */
void car_stop_trunleft()
{
	if (TurnLeft) {
		LeftSpeed = LeftSpeed + 10;
		motor_set_speed(1, LeftSpeed);
		TurnLeft = false;
	}
}

/* 右转, 让右轮速度-10 */
void car_turn_right()
{
	if (!TurnRight) {
		RightSpeed = RightSpeed - 10;
		motor_set_speed(1, RightSpeed);
		TurnRight = true;
	}
}

/* 停止右转, 让右轮速度+10 */
void car_stop_trunright()
{
	if (TurnRight) {
		RightSpeed = RightSpeed + 10;
		motor_set_speed(1, RightSpeed);
		TurnRight = false;
	}
}