#pragma once

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>

#include "i2c.hpp"

class SSD1306
{

private:
    I2C m_i2c;
    uint8_t m_x = 0, m_y = 0, m_fontSize = 0;

    void writeChar(const char c);
    void writeStr(char *str);

public:
    SSD1306();

    void clearScreen();
    void fontSize(const uint8_t byte);
    void gotoxy(const uint8_t x, const uint8_t y);

    void writeP(const char *Buffer, ...);

    #define printf(format, args...) writeP(PSTR(format) , ## args)
};
