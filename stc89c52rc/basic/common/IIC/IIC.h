#ifndef __IIC_H__
#define __IIC_H__

int EEPROM_write_byte_no_addr(unsigned char sladr, unsigned char sdata);
int EEPROM_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt);
int EEPROM_read_byte(unsigned char sladr, unsigned char* rdata);
int EEPROM_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt);
#endif