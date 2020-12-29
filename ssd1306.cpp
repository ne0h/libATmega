#include "ssd1306.hpp"
#include "font.hpp"

const static uint8_t ADDR = 0x78;
const static uint8_t COMMAND = 0x00;
const static uint8_t DATA = 0x40;

const static uint8_t SET_CONTRAST_CONTROL = 0x81;
const static uint8_t SET_DISPLAY_ON = 0xAF;
const static uint8_t SET_DISPLAY_OFF = 0xAE;
const static uint8_t SET_DISPLAY_NORMAL = 0xA6;
const static uint8_t SET_DISPLAY_INVERSE = 0xA7;
const static uint8_t ENTIRE_DISPLAY_ON = 0xA4;

const static uint8_t SET_MEMORY_ADDR_MODE = 0x20;
const static uint8_t HORIZONTAL_ADDRESSING_MODE = 0x00;
const static uint8_t VERTICAL_ADDRESSING_MODE = 0x01;
const static uint8_t PAGE_ADDRESSING_MODE = 0x02;

const static uint8_t SET_PAGE_START = 0xB0;
const static uint8_t SET_COLUMN_ADDRESS = 0x21;
const static uint8_t SET_PAGE_ADDRESS = 0x22;

const static uint8_t SET_COM_OUTPUT_NORMAL = 0xC0;
const static uint8_t SET_COM_OUTPUT_REMAPPED = 0xC8;

const static uint8_t SET_DISPLAY_OFFSET = 0xD3;
const static uint8_t SET_DISPLAY_CLOCK = 0xD5;
const static uint8_t SET_PRECHARGE_PERIOD = 0xD9;
const static uint8_t SET_COM_PINS = 0xDA;

const static uint8_t SET_VCOMH_DESELECT = 0xDB;

const static uint8_t SET_LOWER_COLUMN = 0x00;
const static uint8_t SET_HIGHER_COLUMN = 0x10;
const static uint8_t SET_DISPLAY_START_LINE = 0x40;

const static uint8_t SET_SEGMENT_REMAP_O1 = 0xA0;
const static uint8_t SET_SEGMENT_REMAP_O2 = 0xA1;

const static uint8_t SET_MULTIPLEX_RATIO = 0xA8;
const static uint8_t COMMAND_NO_OPERATION = 0xE3;

const static uint8_t CHARGE_BUMB_SETTING = 0x8D;

const uint8_t cmd_init[] = {
    COMMAND,
    SET_DISPLAY_OFF,
    COMMAND_NO_OPERATION,
    SET_MEMORY_ADDR_MODE, HORIZONTAL_ADDRESSING_MODE,
    SET_PAGE_START,
    SET_COM_OUTPUT_REMAPPED,
    SET_LOWER_COLUMN,
    SET_HIGHER_COLUMN,
    SET_DISPLAY_START_LINE,
    ENTIRE_DISPLAY_ON,
    SET_SEGMENT_REMAP_O2,
    SET_DISPLAY_NORMAL,
    SET_CONTRAST_CONTROL, 0xFF,
    SET_MULTIPLEX_RATIO, 0x3F,
    SET_DISPLAY_OFFSET, 0x00,
    SET_DISPLAY_CLOCK, 0xF0,
    SET_PRECHARGE_PERIOD, 0x22,
    SET_COM_PINS, 0x12,
    SET_VCOMH_DESELECT, 0x20,
    CHARGE_BUMB_SETTING, 0x14,
    SET_DISPLAY_ON};

SSD1306::SSD1306()
{
    m_i2c.write(0, ADDR, cmd_init, sizeof(cmd_init));
    clearScreen();
}

void SSD1306::clearScreen()
{
    gotoxy(0, 0);
    m_i2c.start(ADDR);
    m_i2c.byte(DATA);
    for (uint16_t i = 0; i < 128 << 4; i++)
    {
        m_i2c.byte(0);
    }
    m_i2c.stop();
    gotoxy(0, 0);
}

void SSD1306::gotoxy(const uint8_t x, const uint8_t y)
{
    m_x = x;
    m_y = y;
    const uint8_t cmd[] = {
        COMMAND,
        (uint8_t)(SET_PAGE_START | y),
        SET_COLUMN_ADDRESS,
        (uint8_t)(x * 8),
        SET_DISPLAY_START_LINE | 0x3F
    };
    m_i2c.write(0, ADDR, cmd, sizeof(cmd));
}

void SSD1306::fontSize(const uint8_t byte)
{
    m_fontSize = byte;
}

void SSD1306::writeChar(const char c)
{
    if (m_fontSize != 0)
    {
        uint8_t tmp = 0;
        gotoxy(m_x, m_y);
        const uint16_t len = 2*2*8+1;
        uint8_t cmd[len];
        cmd[0] = DATA;
        for (uint8_t i = 0; i < 8; i++)
        {
            tmp = pgm_read_byte(&font[(unsigned char)c][i]);
            tmp = tmp & 0x0f;
            tmp = ((tmp & 0x01) * 3) + (((tmp & 0x02) << 1) * 3) + (((tmp & 0x04) << 2) * 3) + (((tmp & 0x08) << 3) * 3);
            cmd[i*2+1] = tmp;
            cmd[i*2+2] = tmp;
        }
        m_i2c.write(0, ADDR, cmd, len);

        gotoxy(m_x, m_y + 1);
        for (uint8_t i = 0; i < 8; i++)
        {
            tmp = pgm_read_byte(&font[(unsigned char)c][i]);
            tmp = (tmp & 0xf0) >> 4;
            tmp = ((tmp & 0x01) * 3) + (((tmp & 0x02) << 1) * 3) + (((tmp & 0x04) << 2) * 3) + (((tmp & 0x08) << 3) * 3);
            cmd[i*2] = tmp;
            cmd[i*2+1] = tmp;
        }
        m_i2c.write(0, ADDR, cmd, len);
        m_x += 2;
        m_y -= 1;
    }
    else
    {
        gotoxy(m_x, m_y);
        const uint16_t len = 8+1;
        uint8_t cmd[len];
        cmd[0] = DATA;
        for (uint8_t i = 0; i < 8; i++)
        {
            cmd[i+1] = pgm_read_byte(&font[(unsigned char)c][i]);
        }
        m_i2c.write(0, ADDR, cmd, len);
        m_x += 1;
    }
}

void SSD1306::writeStr(char *str)
{
    while (*str)
    {
        writeChar(*str++);
    }
}

void SSD1306::writeP(const char *Buffer, ...)
{
    va_list ap;
    va_start(ap, Buffer);

    int format_flag;
    char str_buffer[10];
    char str_null_buffer[10];
    char move = 0;
    char Base = 0;
    int tmp = 0;
    char by;
    char *ptr;

    for (;;)
    {
        by = pgm_read_byte(Buffer++);
        if (by == 0)
        {
            break;
        }

        if (by == '%')
        {
            by = pgm_read_byte(Buffer++);
            if (isdigit(by) > 0)
            {
                str_null_buffer[0] = by;
                str_null_buffer[1] = '\0';
                move = atoi(str_null_buffer);
                by = pgm_read_byte(Buffer++);
            }

            switch (by)
            {
            case 's':
                ptr = va_arg(ap, char *);
                while (*ptr)
                {
                    writeChar(*ptr++);
                }
                break;
            case 'b':
                Base = 2;
                goto ConversionLoop;
            case 'c':
                format_flag = va_arg(ap, int);
                writeChar(format_flag++);
                break;
            case 'i':
                Base = 10;
                goto ConversionLoop;
            case 'o':
                Base = 8;
                goto ConversionLoop;
            case 'x':
                Base = 16;
            ConversionLoop:
                itoa(va_arg(ap, int), str_buffer, Base);
                int b = 0;
                while (str_buffer[b++] != 0)
                {
                };
                b--;
                if (b < move)
                {
                    move -= b;
                    for (tmp = 0; tmp < move; tmp++)
                    {
                        str_null_buffer[tmp] = '0';
                    }
                    str_null_buffer[tmp] = '\0';
                    strcat(str_null_buffer, str_buffer);
                    strcpy(str_buffer, str_null_buffer);
                }
                writeStr(str_buffer);
                move = 0;
                break;
            }
        }
        else
        {
            writeChar(by);
        }
    }
    va_end(ap);
}
