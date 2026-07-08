#include "uart.h"
#include <stdlib.h>

void UART_init(unsigned int ubrr){
    // set the baud rate
    UBRR0H = (unsigned char)(ubrr >> 8); // upper 4 bits are reserved take the lower 4 bits and assign it.
    UBRR0L = (unsigned char)(ubrr);

    // enable receiver and transmitter
    UCSR0B = (0x1 << TXEN0) | (0x1 << RXEN0);
    // 1 stop bit, 8 bit data
    UCSR0C = (0x1 << UCSZ01) | (0x1 << UCSZ00) | (0x1 << USBS0);
}

void UART_Transmit(char data){
    // wait till buffer is empty
    // ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    while (!(UCSR0A & (1 << UDRE0)));
    // write the data into buffer
    UDR0 = data;
}

void UART_Transmit_string(const char *str){
    while (*str){
        UART_Transmit(*str++);
    }
    // UART_Transmit('\n');
}

void UART_Transmit_ulong(unsigned long data){
    char buf[11];
    ultoa(data, buf, 10);
    for (char *p = buf; *p; p++){
        UART_Transmit(*p);
    }
    UART_Transmit('\n');
}

unsigned char UART_Receive(void){
    // wait till data is received
    while (!(UCSR0A & (1 << RXC0)));
    // get the data from buffer and return it
    return UDR0;
}
