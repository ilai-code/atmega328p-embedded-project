#ifndef DELAY_H
#define DELAY_H

#include <config.h>
#include <avr/interrupt.h>

extern volatile unsigned long system_ticks;

void timer1_init(void);

ISR(TIMER1_COMPA_vect);

void delay_ms(unsigned long ms); // Take in time in milliseconds


void delay_s(unsigned long s); // Take in time in seconds

unsigned long getTime(); // returns the current system tick;

#endif