#ifndef __IIC_H__
#define __IIC_H__
#include "stdint.h"

void IIC_dev_scan();

int EEPROM_write_byte_no_addr(unsigned char sladr, unsigned char sdata);
int EEPROM_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt);
int EEPROM_read_byte(unsigned char sladr, unsigned char* rdata);
int EEPROM_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt);

void OLED_init();
void OLED_clear();
void OLED_light_one_byte(uint8_t page, uint8_t column);
void OLED_show_char(uint8_t char_row, uint8_t char_column, unsigned char readychar[][16]);
#endif