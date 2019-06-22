#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

/**
 * USART allows the communication with external devices via serial communication.
 */
class USART {

public:
    /**
     * Basic constructor to create a new USART with baudrate 57600. This is default for Arduino Nano.
     */
    USART() {
        USART(57600);
    }

    /**
     * Constructor which allows to set a custom baudrate.
     */
    USART(uint32_t baudrate) {
        uint8_t ubbr = F_CPU / 16 / baudrate - 1;

        // set baudrate
        UBRR0H = (unsigned char)(ubbr >> 8);
        UBRR0L = (unsigned char) ubbr;
        // enable receiver and transmitter
        UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    }

    /**
     * Receive waits for any char being transmitted. Blocks until the char is returned.
     * \return Returns the received char.
     */
    unsigned char receive() {
        while(!(UCSR0A & (1<<RXC0)));
        return UDR0;
    }

    /**
     * Receive waits until a string (char array) is transmitted. Blocks until a zero-byte ('0x00') is received.
     * \param buffer The buffer where the result is stored.
     * \param buffer_length The size of the buffer.
     * \return Returns the number of received chars.
     */
    uint32_t receive(unsigned char* buffer, uint32_t buffer_length) {
        return receive(buffer, buffer_length, 0x00);
    }

    /**
     * Receive waits until a string (char array) is transmitted. Blocks until a zero-byte ('0x00') is received.
     * Allows to set a custom terminal symbole.
     * \param buffer The buffer where the result is stored.
     * \param buffer_length The size of the buffer.
     * \param terminator The custom terminal symbole.
     * \return Returns the number of received chars.
     */
    uint32_t receive(unsigned char* buffer, uint32_t buffer_length, unsigned char terminator) {
        memset(buffer, 0x00, buffer_length);
        uint32_t latest = 0;
        while (latest < buffer_length) {
            unsigned char c = receive();
            buffer[latest] = c;
            latest++;

            if (c == terminator) {
                break;
            }
        }
        return latest;
    }

    /**
     * Send sends a single char.
     * \param c The char to send.
     */
    void send(char c) {
        while(!(UCSR0A & (1<<UDRE0)));
        UDR0 = c;
    }

    /**
     * Send sends a string (char array).
     * \param data The string to send.
     */
    void send(char* data){
        while (*data) {
            send(*data++);
        }
    }

    /**
     * Sends the string representative of an integer as decimal.
     * \param value The integer.
     */
    void sendAsDecChar(const uint16_t value) {
        char buf[6];
        itoa(value, buf, 10);
        send(buf);
    }

    /**
     * Sends the string representative of an integer as hexadecimal.
     * \param value The integer.
     */
    void sendAsHexChar(const uint16_t value) {
        char buf[6];
        itoa(value, buf, 16);
        send(buf);
    }

};

/**
 * USART allows asynchronous communication with external devices via serial communication.
 * The receiver does not block, but is informed via interrupt when a char has been received.
 */
class AsyncUSART : public USART {

public:
    /**
     * Basic constructor to create a new asynchronous USART with baudrate 57600. This is default for Arduino Nano.
     */
    AsyncUSART() {
        AsyncUSART(57600);
    }
    
    /**
     * Constructor which allows to set a custom baudrate.
     */
    AsyncUSART(uint32_t baudrate) : USART(baudrate) {
        // disable synchronous receiver and enable asynchronous receiver (interrupts)
        UCSR0B |= (1 << RXCIE0) | (0<<RXEN0);
    }

};