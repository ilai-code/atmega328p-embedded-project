#include "uart.h"

void USART_init(unsigned int ubrr){
    // set the baud rate
    UBRR0H = (unsigned char) (ubrr >> 8); // upper 4 bits are reserved take the lower 4 bits and assign it.
    UBRR0L = (unsigned char)ubrr;

    // enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // 2 stop bit, 8 bit data
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data){
    // wait till buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));
    // write the data into buffer
    UDR0 = data;
}

void USART_Transmit_ulong(unsigned long data){
    char buf[11];
    ultoa(data, buf, 10);
    for (char *p = buf; *p; p++){
        USART_Transmit(*p);
    }
    USART_Transmit('\n');
}

unsigned char USART_Receive(void){
    // wait till data is received
    while (!(UCSR0A & (1 << RXC0)));
    // get the data from buffer and return it
    return UDR0;
}
