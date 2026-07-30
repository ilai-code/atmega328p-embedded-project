#include <avr/power.h>
#include <FreeRTOS.h>
#include <freeRTOS_library/include/task.h>
#include "uart.h"
#include "lcd.h"
#include "dht.h"
#include "stdio.h"

// #undef DDRB
// #define DDRB *((volatile unsigned char*)0x24)   // Data Direction Register for port B. Refer to data sheet for the address
// #undef PORTB
// #define PORTB *((volatile unsigned char*)0x25)  // Output register for port B. Refer to data sheet for the address

void Port13_Blink(int pinB){
    // flash the led using delay function
    PORTB |= (0x1 << pinB);
    // delay_ms(500);

    PORTB &= ~(0x1 << pinB);
    // delay_ms(500);
}

void BlinkLedPin13Task(){
    // Initialize the current tick
    TickType_t LastTime = xTaskGetTickCount();

    // Set the time in ms to wait
    const TickType_t BlinkPeriod = pdMS_TO_TICKS(1000);

    // Loop to blink the led
    for (;;){
        taskENTER_CRITICAL();
        PORTB ^= (1 << PORTB5);
        taskEXIT_CRITICAL();

        vTaskDelayUntil(&LastTime, BlinkPeriod);
    }
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

void Motor_circuit_logic(){
    // following code is for a dc motor
    // when the button is pressed it will trigger the relay and allow the led light and dc motor to spin
    for (;;){
        taskENTER_CRITICAL();
        int currentButtonState = (int)(PINB & (1 << PINB4));
        taskEXIT_CRITICAL();

        if (currentButtonState == 0){
            taskENTER_CRITICAL();
            PORTB |= (1 << PORTB1);
            taskEXIT_CRITICAL();
        } else {
            taskENTER_CRITICAL();
            PORTB &= ~(1 << PORTB1);
            taskEXIT_CRITICAL();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void Init_Blink(){
    // Set the data direction of port B bit 5 (pin 13) as output
    DDRB |= (0x1 << DDB5);
}

void Init_Motor(){
    DDRB |= (1 << DDB1); // Relay output (Port 9)
    DDRB &= ~(1 << DDB4); // Button input (Port 12)
}

void set_temp_cursor(){
    lcd_setCursor(3, 0);
}

void set_humidity_cursor(){
    lcd_setCursor(3, 1);
}

int main(void){
    // maybe needed to set to 16mhz
    clock_prescale_set(clock_div_1);

    // Setup the register properly for ATMega328p with FreeRTOS
    TIMSK1 |= 0x02;

    // enable the timer
    // timer1_init();

    // enable uart protocol to receive and transmit data
    UART_init(MYUBRR);
    twi_init();
    LCD_begin(16, 2);
    lcd_backlight();
    lcd_setCursor(0, 0);
    lcd_print("T: ");
    lcd_setCursor(0, 1);
    lcd_print("H: ");
    // Init_Blink();
    // Init_Motor();
    DHT sensor;

    // xTaskCreate(BlinkLedPin13Task, "Led_blink", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    // xTaskCreate(Motor_circuit_logic, "Motor_circuit", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    while(1){
        // infinite loop
        dht_read(&sensor);
        if (sensor.status){
            // UART_Transmit_string("data was successfully retrieved\n");
            uint8_t degree_f = ((sensor.temp * 9) / 5) + 32;
            char str[32];
            sprintf(str, "%d C / %d F", sensor.temp, degree_f);
            set_temp_cursor();
            lcd_print(str);
            set_humidity_cursor();
            sprintf(str, "%d%%", sensor.humidity);
            lcd_print(str);

            // sprintf(str, "The temp is: %dC / %dF and the humidity is %d%%\n", sensor.temp, degree_f, sensor.humidity);
            // UART_Transmit_string(str);
        } else {
            lcd_clear();
            lcd_print("error");
        }

        _delay_ms(2000);
    }


    return 0;
}