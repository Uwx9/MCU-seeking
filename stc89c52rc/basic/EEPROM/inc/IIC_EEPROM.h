#ifndef __IIC_H__
#define __IIC_H__

int IIC_write_byte_no_addr(unsigned char sladr, unsigned char sdata);
int IIC_write_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* sendstr, unsigned char cnt);
int IIC_read_byte(unsigned char sladr, unsigned char* rdata);
int IIC_read_str_with_addr(unsigned char sladr, unsigned char subaddr, unsigned char* readed_str, unsigned char cnt);
#endif