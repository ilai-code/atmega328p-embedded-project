#include "delay.h"
#include "util/atomic.h"
#include <stdlib.h>


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

    // long f_cpu = 16000000UL; // Timer tick duration calculated as T_tick = prescaler / f_cpu
    // Num of timer counts would then be N_count = 1 ms / T_tick 
    // or 1000 microsecond / 4 microseconds = 250

    TCCR1B |= (1 << WGM12); // CTC (Clear timer on compare match mode) Mode
    // can write to the the low and high bits individually or you can do it together but make sure the data type is correct for the size
    OCR1AH = 0;
    OCR1AL = ((F_CPU / PRESCALE) / 1000UL) - 1UL; 
    // OCR1A = ((F_CPU / PRESCALE) / 1000UL) - 1UL; // set the timer tick of the output compare register (lower bit since 249)

    TCCR1B |= (1 << CS11) | (1 << CS10);

    TIMSK1 |= (1 << OCIE1A);

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