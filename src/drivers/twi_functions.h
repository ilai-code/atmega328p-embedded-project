#ifndef TWI_FUNCTIONS_H
#define TWI_FUNCTIONS_H

#include <avr/io.h>
#include <util/twi.h>

// define the scl/sda pins
#define TW_SCL_PIN PORTC5
#define TW_SDA_PIN PORTC4

#define DEBUG 0

#define TWI_SLA_W(ADDR) (ADDR | TW_WRITE)
#define TWI_SLA_R(ADDR) (ADDR | TW_READ)

void twi_init();
uint16_t twi_send(uint8_t slave_addr, uint8_t* data, uint8_t len);
uint16_t twi_recieve(uint8_t slave_addr, uint8_t* data, uint8_t len);

#endif