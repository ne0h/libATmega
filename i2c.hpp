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

    uint8_t start(const uint8_t addr);

    uint8_t byte(uint8_t byte);

    void stop(void);

    int8_t read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

    int8_t write(uint8_t dev_id, const uint8_t reg_addr, const uint8_t *reg_data, const uint16_t len);
};
