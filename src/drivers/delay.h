#ifndef __DELAY_H__
#define __DELAY_H__

#include <config.h>
#include <avr/interrupt.h>

extern volatile unsigned long system_ticks;

void timer1_init(void);

ISR(TIMER1_COMPA_vect);

void delay_ms(unsigned long ms); // Take in time in milliseconds

unsigned long getTime(); // returns the current system tick;

#endif