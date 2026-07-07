#ifndef __UART_H__
#define __UART_H__

#include <config.h>

#define MYUBRR F_CPU / 16 / BAUD-1 // calculate the baud rate register setting (asynchronous normal mode)

void USART_init(unsigned int ubrr);

void USART_Transmit(unsigned char data);

void USART_Transmit_ulong(unsigned long data);

unsigned char USART_Receive(void);



#endif