#ifndef __IIC_H__
#define __IIC_H__

void IIC_init();
void IIC_start();
void IIC_stop();
void IIC_send_byte(uint8_t sendbyte);
uint8_t IIC_receive_byte();
void IIC_send_ack(uint8_t ack_bit);
uint8_t IIC_receive_ack();

#endif