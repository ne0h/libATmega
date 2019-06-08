#pragma once

#include <avr/io.h>
#include <util/delay.h>

#define SDA_PIN     PC4
#define SCL_PIN		PC5
#define I2C_PORT	PORTC
#define SCL_CLOCK	400000UL

class I2C {

public:
    I2C();

    uint8_t start(const uint8_t addr);

    uint8_t byte(uint8_t byte);

    void stop(void);

};
