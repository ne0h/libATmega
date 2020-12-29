#pragma once

#include <avr/io.h>
#include <util/delay.h>

#define SDA_PIN PC4
#define SCL_PIN PC5
#define I2C_PORT PORTC
#define SCL_CLOCK 400000UL

class I2C
{

public:
    I2C();

    int8_t start(const uint8_t addr);

    int8_t writeByte(uint8_t byte);

    int8_t readByte(uint8_t *byte, const bool ack);

    void stop(void);

    int8_t read(uint8_t dev_id, uint8_t addr, uint8_t *data, uint16_t len);

    int8_t write(uint8_t dev_id, const uint8_t addr, const uint8_t *data, const uint16_t len);

};
