#include <avr/power.h>
#include <FreeRTOS.h>
#include <freeRTOS_library/include/task.h>

// #undef DDRB
// #define DDRB *((volatile unsigned char*)0x24)   // Data Direction Register for port B. Refer to data sheet for the address
// #undef PORTB
// #define PORTB *((volatile unsigned char*)0x25)  // Output register for port B. Refer to data sheet for the address

// set port 9 to be output (the relay) and port 12 to be input (the button)
#define motor_setup 0b00000010

void Port13_Blink(int pinB){
    // flash the led using delay function
    PORTB |= (0x1 << pinB);
    // delay_ms(500);

    PORTB &= ~(0x1 << pinB);
    // delay_ms(500);

    return;
}

void BlinkLedPin13Task(){
    // Set the data direction of port B bit 5 (pin 13) as output
    DDRB |= (0x1 << DDB5);

    // Initialize the current tick
    TickType_t LastTime = xTaskGetTickCount();

    // Set the time in ms to wait
    const TickType_t BlinkPeriod = pdMS_TO_TICKS(1000);

    // Loop to blink the led
    for (;;){
        vTaskDelayUntil(&LastTime, BlinkPeriod);
        PORTB ^= (1 << PB5);

        // for (volatile uint32_t i = 0; i < 100000; i++);
    }

    return;
}

//@brief flashes led in an alternating fashion
//@param pinD enter the lower of the 2 portD pins. i.e 3 for pin 3 & 4
void double_led_pwm(int pinD){
    DDRD |= (0x3 << pinD);

    PORTD = (0x1 << pinD);
    // delay_ms(100);

    PORTD = (0x2 << pinD);
    // delay_ms(100);
}

void led_array(){
    // led array pin layout from left to right PB5  | PB4  | PB3  | PB2  | PB1 | PB0 | PD7 | PD6 | PD5 | PD4 | PD3 |
    //                                          13  |  12  |  11  |  10  |  9  |  8  |  5  |  4  |  3  |  2  |  1  |
    // set pins 0-4
    DDRB |= 0x1F;
    // set pins 3-7
    DDRD |= (0x3F << 2);

    int PB = 4;
    int PD = 7;

    for (;PB >= 0; PB--){
            PORTB = (0x1 << PB);
            // delay_ms(100);
            PORTB = ~(0x1 << PB);
    }

    PORTB = 0;
    
    for (; PD >= 0; PD--){
        PORTD = (0x1 << PD);
        // delay_ms(100);
        PORTD = ~(0x1 << PD);
    }
    
    PORTD = 0;
}

void motor_circuit_logic(){
    // following code is for a dc motor
    // when the button is pressed it will trigger the relay and allow the led light and dc motor to spin
    int relayState = 0x0;
    int currentButtonState = (int)(PINB & (1 << PINB4));

    if (currentButtonState == 0x0){
        PORTB = ~(relayState << PORTB1);
    } else {
        PORTB = (relayState << PORTB1);
    }
}


int main(void){
    // maybe needed to set to 16mhz
    clock_prescale_set(clock_div_1);

    // Setup the register properly for ATMega328p
    TIMSK1 |= 0x02;

    // enable the timer
    // timer1_init();

    // enable uart protocol to receive and transmit data
    // UART_init(MYUBRR);

    xTaskCreate(BlinkLedPin13Task, "Led_blink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    // DDRB = motor_setup;

    // delay_s(5);
    // DDRB |= (1 << DDB5);

    while(1){
        // delay_s(1);
        // UART_Transmit_string("1 seconds passed\n");
    }


    return 0;
}