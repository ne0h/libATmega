#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include <common.hpp>

const static uint16_t TIMER8_MAX  = 255;
const static uint16_t TIMER16_MAX = 1999;

/**
 * LEDId identifies a LED.
 */
enum LEDId {
    LED0,
    LED1,
    LED2,
    LED3,
};

/**
 * LED encapsulates a LED.
 */
class LED {

private:
    const LEDId m_id;

public:
    /**
     * Creates a new LED.
     * \param id Identifies the LED
     */
    LED(const LEDId id) : m_id(id) {}

    /**
     * Sets the brightness of the LED.
     * \param brightness The brigthness.
     */
    void set(const float brightness) {
        switch (m_id) {
            case LED0: OCR0A = TIMER8_MAX  * brightness; break;
            case LED1: OCR0B = TIMER8_MAX  * brightness; break;
            case LED2: OCR1A = TIMER16_MAX * brightness; break;
            case LED3: OCR1B = TIMER16_MAX * brightness; break;
        }
    }
};

/**
 * LEDManager initiates the timer-based PWM channels.
 */
class LEDManager {

private:
    static LEDManager* m_instance;

    // make constructors private
    LEDManager() {
        // init timer0: - fast PWM mode with pre-scaler=64
        //              - frequency is set to 1000 Hz
        TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
        TCCR0B |= (1<<CS01)   | (1<<CS00);
        DDRD   |= (1<<PD5) | (1<<PD6);

        // init timer1: - fast PWM mode with pre-scaler=8
        //              - frequency is set to 1000 Hz
        TCCR1A |= (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);
        TCCR1B |= (1<<WGM12) | (1<<WGM13)  | (1<<CS11);
        DDRB   |= (1<<PB1)   | (1<<PB2);
        ICR1 = TIMER16_MAX;
    }
    LEDManager(const LEDManager&);
    ~LEDManager() {}

    class CGuard {
        public:
            ~CGuard() {
                if (LEDManager::m_instance != NULL) {
                    delete m_instance;
                    LEDManager::m_instance = NULL;
                }
            }
    };

public:
    /**
     * Returns a pointer to the instance of LEDManager.
     * \return a pointer to the instance of LEDManager.
     */
    static LEDManager* getInstance() {
        static CGuard g;
        if (!m_instance) {
            m_instance = new LEDManager();
        }
        return m_instance;
    }

    /**
     * Returns the LED identified by the corresponding LEDId.
     * \param id The identifier of the LED.
     * \return The LED identified by the corresponding LEDId.
     */
    LED get(LEDId id) {
        return LED(id);
    }

};

LEDManager* LEDManager::m_instance = 0; // static class elements need to be initialized

/**
 * RGBLED holds a colored LED.
 */
class RGBLED {

private:
    LED m_r;
    LED m_g;
    LED m_b;

public:
    /**
     * Instanziates a new RGBLED.
     * \param r Identifies the red LED.
     * \param g Identifies the green LED.
     * \param b Identifies the blue LED.
     */
    RGBLED(const LED r, const LED g, const LED b) : m_r(r), m_g(g), m_b(b) {}

    /**
     * Updates the color.
     * \param r New value of the red LED.
     * \param g New value of the green LED.
     * \param b New value of the blue LED.
     */
    void set(const float r, const float g, const float b) {
        m_r.set(r);
        m_g.set(g);
        m_b.set(b);
    }

};
