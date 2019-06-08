#include "i2c.hpp"

I2C::I2C() {
	I2C_PORT |= (1<<SDA_PIN) | (1<<SCL_PIN);
	TWCR = 0;
	TWSR = 0;
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;
	_delay_ms(50);
}

uint8_t I2C::start(const uint8_t addr) {
	uint8_t timeout = 0;

    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    while(!(TWCR & (1<<TWINT))) {
	    if ((timeout++) > 100) {
            return 1;
        }
    }
    TWDR = addr;
    TWCR = (1<<TWINT) | (1<<TWEN);
	timeout = 0;
    while (!(TWCR & (1<<TWINT))) {
	    if ((timeout++) > 100) {
            return 1;
        }
    }
	return 0;
}

uint8_t I2C::byte(uint8_t byte) {  
	uint8_t timeout = 0;

    TWDR = byte;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT))) {
		if ((timeout++) > 100) {
            return 1;
        }
	}
	return 0;
}

void I2C::stop() {
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	TWCR = 0;
}
