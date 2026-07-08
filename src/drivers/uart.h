#ifndef __UART_H__
#define __UART_H__

#include <config.h>

#define MYUBRR (F_CPU / (16UL * BAUD)) - 1UL // calculate the baud rate register setting (asynchronous normal mode)

void UART_init(unsigned int ubrr);

void UART_Transmit(char data);

void UART_Transmit_string(const char *str);

void UART_Transmit_ulong(unsigned long data);

unsigned char UART_Receive(void);



#endif