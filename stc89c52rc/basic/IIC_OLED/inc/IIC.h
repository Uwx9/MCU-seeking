#ifndef __IIC_H__
#define __IIC_H__
#include "stdint.h"

enum OLED_roll_direction {
	OLED_roll_up,
	OLED_roll_down,
	OLED_roll_left,
	OLED_roll_right
};

void IIC_dev_scan();

int EEPROM_write_byte_no_addr(unsigned char sladr, unsigned char sdata);
int EEPROM_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt);
int EEPROM_read_byte(unsigned char sladr, unsigned char* rdata);
int EEPROM_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt);

void OLED_init();
void OLED_clear();
void OLED_light_one_byte(uint8_t page, uint8_t column);
void OLED_show_char_16x16(uint8_t char_row, uint8_t char_column, unsigned char readychar[][16]);
void OLED_show_char_8x16(uint8_t char_row, uint8_t char_column, unsigned char ascill, const unsigned char code*  readychar);
void OLED_roll(uint8_t direction);
#endif