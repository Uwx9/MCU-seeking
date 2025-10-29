/**
 * @file PWM.c
 * @author Unique9
 * @brief TIM3_CH1输出PWM波形
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __PWM_LED_H__
#define __PWM_LED_H__
#include "stm32f10x.h"

void PWMTIM3_init();
void PWMTIM3_set_compare(uint16_t CCRx, uint16_t compare);
void PWNTIM3_set_psc(uint16_t psc);
#endif