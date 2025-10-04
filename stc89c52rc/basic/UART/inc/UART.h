#ifndef _UART_H_
#define _UART_H_

void UART_init();
void UART_send_byte(unsigned char Byte);
void UART_send_string(unsigned char* sendstr);
#endif