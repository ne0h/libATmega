#include "i2c.hpp"

#define COMMAND 0x00

I2C::I2C()
{
    I2C_PORT |= (1 << SDA_PIN) | (1 << SCL_PIN);
    TWCR = 0;
    TWSR = 0;
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
    _delay_ms(50);
}

int8_t I2C::start(const uint8_t addr)
{
    uint8_t timeout = 0;

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
    {
        if ((timeout++) > 100)
        {
            return 1;
        }
    }

    return writeByte(addr);
}

int8_t I2C::writeByte(uint8_t byte)
{
    uint8_t timeout = 0;

    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
    {
        if ((timeout++) > 100)
        {
            return 1;
        }
    }
    return 0;
}

int8_t I2C::readByte(uint8_t *byte, const bool ack)
{
    uint8_t timeout = 0;

    TWCR = (1<<TWINT) | (1<<TWEN) | (ack<<TWEA);
    while (!(TWCR & (1<<TWINT)))
    {
        if ((timeout++) > 100)
        {
            return 1;
        }
    }
    uint8_t res = TWDR;
    byte = &res;
    return 0;
}

void I2C::stop()
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    TWCR = 0;
}

int8_t I2C::read(uint8_t dev_id, uint8_t addr, uint8_t *data, uint16_t len)
{
    start(addr);
    for (uint16_t i = 0; i < len; i++)
    {
        if (!readByte(&(data[i]), false)) return 1;
    }
    return 0;
}

int8_t I2C::write(uint8_t dev_id, const uint8_t addr, const uint8_t *data, const uint16_t len)
{
    start(addr);
    for (uint16_t i = 0; i < len; i++)
    {
        writeByte(data[i]);
    }
    stop();
    return 0;
}
