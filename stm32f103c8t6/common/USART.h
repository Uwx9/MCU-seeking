#ifndef __USART_H__
#define __USART_H__

extern uint8_t TXpkg[];
extern char RXpkg[];
extern uint8_t rx_flag;
void MyUSART_init();
void usart_send_byte(uint8_t data);
void usart_send_byte_arr(uint8_t* arr, uint16_t lenth);
void usart_send_string(char* str);
void usart_send_num(uint32_t num, uint8_t lenth);
void usart_printf(char* format, ...);
void usart_send_datapkg();
uint8_t get_usart_flag();


#endif