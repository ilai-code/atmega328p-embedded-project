#include "drivers/delay.h"
#include "drivers/uart.h"

// #undef DDRB
// #define DDRB *((volatile unsigned char*)0x24)   // Data Direction Register for port B. Refer to data sheet for the address
// #undef PORTB
// #define PORTB *((volatile unsigned char*)0x25)  // Output register for port B. Refer to data sheet for the address

void Port13_Blink(int pinB){
    // flash the led using delay function
    PORTB |= (0x1 << pinB);
    delay_ms(500);

    PORTB &= ~(0x1 << pinB);
    delay_ms(500);

    return;
}

//@brief flashes led in an alternating fashion
//@param pinD enter the lower of the 2 portD pins. i.e 3 for pin 3 & 4
void double_led_pwm(int pinD){
    DDRD |= (0x3 << pinD);

    PORTD = (0x1 << pinD);
    delay_ms(100);

    PORTD = (0x2 << pinD);
    delay_ms(100);
}

void led_array(){
    // led array pin layout from left to right PB4 | PB3 | PB2 | PB1 | PB0 | PD7 | PD6 | PD5 | PD4 | PD3 |
    //                                          10 |  9  |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |
    // set pins 0-4
    DDRB |= 0x1F;
    // set pins 3-7
    DDRD |= (0x3F << 2);

    int PB = 4;
    int PD = 7;

    for (;PB >= 0; PB--){
            PORTB = (0x1 << PB);
            delay_ms(100);
            PORTB = ~(0x1 << PB);
    }

    PORTB = 0;
    
    for (; PD >= 0; PD--){
        PORTD = (0x1 << PD);
        delay_ms(100);
        PORTD = ~(0x1 << PD);
    }
    
    PORTD = 0;
}


int main(void){
    // USART_init(MYUBRR);
    timer1_init();

    // initialize port 5 of data direction register as output
    DDRB |= (0x1 << DDB5);

    while(1){
        // Port13_Blink(DDB5);
        // delay_ms(200);
        // double_led_pwm(4);
        // led_array();
        // unsigned long start = system_ticks;
        // unsigned long elapsed = end - start;
        // USART_Transmit_ulong(elapsed);
    }
}