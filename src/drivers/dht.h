#ifndef DHT_H
#define DHT_H

#include <avr/io.h>
#include <util/delay.h>

// using digital pin 10 corresponds PB2 or using the 2nd bit
#define DHT_BIT 2
#define DHT_DDR DDRB
#define DHT_PORT PORTB
#define DHT_PIN PINB


typedef struct {
    uint8_t status;
    uint8_t temp;
    uint8_t humidity;
} DHT;

void dht_read(DHT *dht);

#endif