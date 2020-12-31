#pragma once

#include <avr/io.h>

#define SPI_DDR     DDRB
#define SPI_SCK     PB5
#define SPI_MISO    PB4
#define SPI_MOSI    PB3

/**
 * SPI implements an interface to the SPI bus.
 */
class SPI {

public:
    
    /**
     * Default constructor which enables SPI.
     */
    SPI() {
        // Source: _C Code Example_ on page 137 in the datasheet

        // Set MOSI and SCK output, all others input
        SPI_DDR = (1<<SPI_MOSI) | (1<<SPI_SCK);
        // Enable SPI, Master, set clock rate fck/16
        SPCR = (1<<SPE) | (1<<MSTR);
    };

    /**
     * Start starts the transfer by setting the CS pin to _low_.
     * \param port The port of the pin.
     * \param pin  The pin.
     */
    void start(volatile uint8_t *port, const uint8_t pin) {
        *port &= ~(1<<pin);
    }

    /**
     * Read reads one byte from the SPI bus.
     * \return Returns the byte.
     */
    uint8_t read() {
        SPDR = 0x00;
        while (!(SPSR & (1<<SPIF)));
        return SPDR;
    }

    /**
     * Write writes one byte to the SPI bus.
     * \param byte The byte to send.
     */
    void write(const uint8_t byte) {
        SPDR = byte;
        while (!(SPSR & (1<<SPIF)));
    }

    /**
     * Stop stops the transfer by setting the CS pin to _high_.
     * \param port The port of the pin.
     * \param pin  The pin.
     */
    void stop(volatile uint8_t *port, const uint8_t pin) {
        *port |= (1<<pin);
    }

};
