/**
 * @file led.c
 * @author your name (you@domain.com)
 * @brief led0接PA0口, led1接PA1口, IO口输出低电平点亮
 * @version 0.1
 * @date 2025-10-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __LED_H__
#define __LED_H__

void led_init();
void led0_on();
void led0_off();
void led0_turn();
void led1_on();
void led1_off();
void led1_turn();
#endif