#pragma once

#include <avr/io.h>

/*
 * ServoNumber describes one concret instance. As ATmega328p has one 16bit timer, two servos can be controlled via one
 * ATmega328p.
 */
enum ServoNumber {
    SERVO1A,
    SERVO1B,
};

/*
 * Servo controls a SG90 servo via PWM.
 */
class Servo {

private:
    const ServoNumber m_number;
    uint16_t m_value;

public:
    /*
     * Creates a new servo.
     */
    Servo(const ServoNumber number) : m_number(number) {

        // Fast PWM mode with pre-scaler=8
        TCCR1A |= (1<<WGM11);
        TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11);

        // set comparator to equivalent of 20ms (50Hz)
        ICR1 = 39999;

        // activate corresponding output
        switch (m_number) {
            case SERVO1A:
	            TCCR1A |= (1<<COM1A1);
                DDRB   |= (1<<PINB1);
                break;
            case SERVO1B:
                TCCR1A |= (1<<COM1B1);
                DDRB   |= (1<<PINB2);
                break;
        }

        setValue(500);
    }

    /**
     * Returns the latest position.
     */
    const uint16_t getValue() {
        return m_value;
    }

    /**
     * Sets the position. Ignores values less then zero are bigger than 1000.
     */
    void setValue(const uint16_t value) {
        if (value < 0 || value > 1000) {
            return;
        }

        m_value = 3760.f * value / 1000 + 1040;       // 4800-1040=3760
        switch (m_number) {
            case SERVO1A:
	            OCR1A = m_value;
                break;
            case SERVO1B:
                OCR1B = m_value;
                break;
        }
    }

};
