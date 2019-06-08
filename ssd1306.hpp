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

#define OLED_I2C_ADDR				0x78

#define COMMAND						0x00
#define DATA						0x40

#define SET_CONTRAST_CONTROL		0x81
#define SET_DISPLAY_ON				0xAF
#define SET_DISPLAY_OFF				0xAE
#define SET_DISPLAY_NORMAL			0xA6
#define SET_DISPLAY_INVERSE			0xA7
#define ENTIRE_DISPLAY_ON			0xA4
	
#define SET_MEMORY_ADDR_MODE		0x20
#define HORIZONTAL_ADDRESSING_MODE	0x00
#define VERTICAL_ADDRESSING_MODE	0x01
#define PAGE_ADDRESSING_MODE		0x02
		
#define SET_PAGE_START				0xB0
#define SET_COLUMN_ADDRESS			0x21
#define SET_PAGE_ADDRESS			0x22
	
#define SET_COM_OUTPUT_NORMAL		0xC0
#define SET_COM_OUTPUT_REMAPPED		0xC8
	
#define SET_DISPLAY_OFFSET			0xD3
#define SET_DISPLAY_CLOCK			0xD5
#define SET_PRECHARGE_PERIOD		0xD9
#define SET_COM_PINS				0xDA
	
#define SET_VCOMH_DESELECT			0xDB
	
#define	SET_LOWER_COLUMN			0x00
#define	SET_HIGHER_COLUMN			0x10
#define	SET_DISPLAY_START_LINE		0x40
	
#define SET_SEGMENT_REMAP_O1		0xA0	
#define SET_SEGMENT_REMAP_O2		0xA1
	
#define SET_MULTIPLEX_RATIO			0xA8
#define COMMAND_NO_OPERATION		0xE3
	
#define CHARGE_BUMB_SETTING			0x8D

class SSD1306 {

private:
	I2C m_i2c;
	uint8_t m_x = 0, m_y = 0, m_fontSize = 0;

	void writeChar(const char c);
	void writeStr(char* str);

public:
	SSD1306();

	void clearScreen();
	void fontSize(const uint8_t byte);
	void gotoxy(const uint8_t x, const uint8_t y);

	void writeP(const char *Buffer, ...);

	#define write(format, args...) writeP(PSTR(format) , ## args)
};
