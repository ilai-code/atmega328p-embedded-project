#include "twi_functions.h"

// The code here references this git repo: https://github.com/Sovichea/avr-i2c-library/blob/master/twi

void twi_init(){
    // set the scl/sda pins to be output mode temporarily
    DDRC |= (1 << TW_SCL_PIN) | (1 << TW_SDA_PIN);

    // disable pull up resistor
    PORTC &= ~((1 << TW_SCL_PIN) | (1 << TW_SDA_PIN));

    // set the scl/sda pins back to input mode
    DDRC &= ~((1 << TW_SCL_PIN) | (1 << TW_SDA_PIN));
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); // Prescaler set to 1 since it fits into 8 bits
    TWBR = 72; // (((F_CPU / 100000UL) - 16) / 2) max scl for lcd is 100 Khz
}

static uint16_t twi_start(){
    // send start condition
    TWCR = (1 << TWINT)|(1 << TWSTA)| (1 << TWEN);

    // wait for TWINT flag to set to show START condition has been transmitted
    while (!(TWCR & (1 << TWINT)));

    // check for error
    if (TW_STATUS != TW_START){
        return TW_STATUS;
    }

    return 0;
}

static void twi_stop(){
    // transmit stop
    TWCR = (1 << TWINT)|(1 << TWEN)| (1 << TWSTO);
}

static uint16_t twi_prepare_slave_rw(uint8_t slave_addr){
    // transmit the slave address and set the R/W bit
    TWDR = slave_addr;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // wait for TWINT flag to set
    while (!(TWCR & (1 << TWINT)));

    // error check
    if (TW_STATUS != TW_MT_SLA_ACK){ 
        return TW_STATUS;
    }

    return 0;
}

static uint16_t twi_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT)));

    if (TW_STATUS != TW_MT_DATA_ACK){
        return TW_STATUS;
    }

    return 0;
}

static uint8_t twi_read(uint8_t readAck){
    if (readAck){
        // set TWEA to generate ACK
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

        while (!(TWCR & (1 << TWINT)));

        if (TW_STATUS != TW_MR_DATA_ACK){
            return TW_STATUS;
        }
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)));

        if (TW_STATUS != TW_MR_DATA_ACK){
            return TW_STATUS;
        }
    }

    uint8_t data = TWDR;

    return data;
}

// @param slave_addr must be passed in as (addr << 1)
uint16_t twi_send(uint8_t slave_addr, uint8_t* data, uint8_t len){
    uint16_t error;

    error = twi_start();
    if (error != 0){
        return error;
    }

    error = twi_prepare_slave_rw(TWI_SLA_W(slave_addr));
    if (error != 0){
        return error;
    }

    for (int i = 0; i < len; i++){
        error = twi_write(data[i]);
        if (error != 0){
            return error;
        }
    }

    twi_stop();

    return 0;
}

// @param slave_addr must be passed in as (addr << 1)
uint16_t twi_recieve(uint8_t slave_addr, uint8_t* data, uint8_t len){
    uint16_t error;

    error = twi_start();
    if (error != 0){
        return error;
    }

    error = twi_prepare_slave_rw(TWI_SLA_R(slave_addr));
    if (error != 0){
        return error;
    }

    for (int i = 0; i < len - 1; i++){
        data[i] = twi_read(1);
    }
    data[len - 1] = twi_read(0);

    twi_stop();

    return 0;
}