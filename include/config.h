#ifndef CONFIG_H
#define CONFIG_H
#pragma once

#include <avr/io.h>
#include "util/atomic.h"

// just in case not defined
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define PRESCALE 64UL // define the prescale value
#define BAUD 9600UL // define the baud rate




#endif