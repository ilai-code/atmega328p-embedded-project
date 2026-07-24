#include "delay.h"


// examples of how to define the physical locations of registers or other values
// #undef TCCR1A
// #define TCCR1A *((volatile unsigned char*)0x80)
// #undef TCCR1B
// #define TCCR1B *((volatile unsigned char*)0x81)
// #undef OCR1A
// #define OCR1A *((volatile unsigned short*)0x88)
// #undef OCR1AL
// #define OCR1AL *((volatile unsigned char*)0x88)
// #undef OCR1AH
// #define OCR1AH *((volatile unsigned char*)0x89)
// #undef TIMSK1
// #define TIMSK1 *((volatile unsigned char*)0x6F)
// #undef OCIE1A
// #define OCIE1A 1
// #undef WGM12
// #define WGM12  3
// #undef CS10
// #define CS10   0
// #undef CS11
// #define CS11   1
// #undef CS12
// #define CS12   2

#define s_per_tick 1000UL

volatile unsigned long system_ticks = 0;

void timer1_init(){
    TCCR1A = 0; // Clear the registers for normal operation
    TCCR1B = 0; // Clear the registers for normal operation

    TCCR1B |= (1 << WGM12); // CTC (Clear timer on compare match mode) Mode
    // can write to the the low and high bits individually or you can do it together but make sure the data type is correct for the size
    // OCR1AH = 0;

    // Prescale is a value that divides the clock frequency by a certain amount to make it more manageable
    // Examples of prescale values are 1, 8, 64, 256, or 1024
    // OCR1A basically tells how many ticks to trigger an interrupt
    // This means that we need to set OCR1A to some sort of unit like ticks / unit of time
    // OCR1A itself is a 16 bit register meaning that it can hold up to a value of 0xFFFF (65535) or if you split it between the high and low register it would each hold 0xFF (255)
    // This also means that you can't just utilize the F_CPU of 16 Mhz since it is way above the allowable value
    // Choosing the right prescaler depends on what unit of time we want to be accurate to
    // I.E. if we want to just be accurate to the nearest second then (16 Mhz / 256) - 1 = 62499 (0xF423) or (16 Mhz / 1024) - 1 = 15624 (0x3D08) which both fits into the OCR1A register
    // We substract by 1 because the timer counts from 0. Note that it is best to choose the smallest prescaler for accuracy and granularity
    // We want 1 ms timer so 1000 ms = 1 s and if we divide the various prescaler with our clock speed we find that 64 gives us a whole number for the number of ticks
    // Basically (F_CPU / Prescale) / 1000UL gives us 250 when prescale is 64 so that is what we use
    // You can separately assign the high and low register but since avr gives us a macro for the combined 16 bit register were just going to use that
    // Note that if you separate the high and low that it is recommended to set the high register before you set the low register (at least I think it is like this based on what I read from datasheet)
    OCR1A = ((F_CPU / PRESCALE) / 1000UL) - 1UL;

    // Select the clock source (prescaler) to be used by the timer/counter (check datasheet for details)
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // TCCR1B |= (1 << CS12);

    // Enable Timer/Counter1 output compare A
    TIMSK1 |= (1 << OCIE1A);

    // Enable global interrupt
    sei();

}

ISR(TIMER1_COMPA_vect){
    system_ticks++;
}

// atomic function to read the system_tick properly
void delay_ms(unsigned long ms){
    unsigned long start;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        start = system_ticks;
    }

    while (1)
    {
        // wait
        unsigned long current_ticks;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            current_ticks = system_ticks;
        }

        if ((current_ticks - start) >= ms){
            break;
        }

    }
}

void delay_s(unsigned long s){
    unsigned long start;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        start = system_ticks;
    }

    while (1)
    {
        // wait
        unsigned long current_ticks;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
            current_ticks = system_ticks;
        }

        if ((current_ticks - start) >= (s * 1000)){
            break;
        }

    }
}

unsigned long getTime(){
    unsigned long time;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        time = system_ticks;
    }

    return time;
}