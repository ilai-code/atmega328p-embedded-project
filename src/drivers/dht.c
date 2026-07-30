#include "dht.h"

static uint8_t data[5];



void dht_start(){
    DHT_DDR |= (1 << DHT_BIT); // set pin as output
    DHT_PORT &= ~(1 << DHT_BIT); // pull the pin low
    _delay_ms(18); // needs to wait at least 18 ms

    DHT_PORT |= (1 << DHT_BIT); // pull the pin high
    _delay_us(40); // needs to wait 20-40 us 
    DHT_DDR &= ~(1 << DHT_BIT); // set the pin as input to listen to DHT11
}


uint8_t dht_response(){
    _delay_us(40);

    // check if the pin is pulled low
    if (!(DHT_PIN & (1 << DHT_BIT))){
        _delay_us(80); // wait out the initial response signal
        
        // check if the line was pulled high
        if (DHT_PIN & (1 << DHT_BIT)){
            _delay_us(40); // wait out the remainder 80 us pulse
            return 1; // response ok
        }
    }

    return 0; // response error/timeout
}

uint8_t dht_read_byte(){
    uint8_t result = 0;
    
    for (uint8_t i = 0; i < 8; i++){
        while (!(DHT_PIN & (1 << DHT_BIT))); // wait the 50 us initial low voltage level
        
        _delay_us(30); // wait a certain amount of time in order to know whether the signal is 0 or 1
        
        // if the line remains high after 30 us then it indicates 1 otherwise 0
        if (DHT_PIN & (1 << DHT_BIT)){
            result = (result << 1) | 0x01; // set the bit to 1
            while (DHT_PIN & (1 << DHT_BIT)); // wait out the remaining high signal
        } else {
            result = (result << 1); // set the bit to 0
        }
    }

    return result;
}

void dht_read(DHT *dht){
    dht_start();
    
    if (dht_response()){
        data[0] = dht_read_byte(); // integral RH data
        data[1] = dht_read_byte(); // decimal RH data
        data[2] = dht_read_byte(); // integral T data
        data[3] = dht_read_byte(); // decimal T data
        data[4] = dht_read_byte(); // check sum

        if (data[4] == (data[0] + data[1] + data[2] + data[3])){
            dht->status = 1;
            dht->temp = data[2];
            dht->humidity = data[0];
            return;
        } else {
            dht->status = 0;
            return;
        }
    }

    dht->status = 0;
    return;
}