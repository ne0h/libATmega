#include "ssd1306.hpp"
#include "font.hpp"

const uint8_t ssd1306_init_cmd [] PROGMEM = {
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
	SET_CONTRAST_CONTROL,	0xFF,
	SET_MULTIPLEX_RATIO,	0x3F,
	SET_DISPLAY_OFFSET,		0x00,
	SET_DISPLAY_CLOCK,		0xF0,
	SET_PRECHARGE_PERIOD,	0x22,
	SET_COM_PINS,			0x12,
	SET_VCOMH_DESELECT,		0x20,
	CHARGE_BUMB_SETTING,	0x14,
	SET_DISPLAY_ON
};

SSD1306::SSD1306() {
	m_i2c.start(OLED_I2C_ADDR);
	m_i2c.byte(COMMAND);
	for (uint8_t tmp = 0; tmp< sizeof(ssd1306_init_cmd); tmp++) {	
		m_i2c.byte(pgm_read_byte(&ssd1306_init_cmd[tmp]));
	}
	m_i2c.stop();
    clearScreen();
}

void SSD1306::clearScreen() {
	gotoxy(0,0);
	m_i2c.start(OLED_I2C_ADDR);
    m_i2c.byte(DATA);
    for (uint16_t i = 0; i < 128<<4 ; i++) {
        m_i2c.byte(0);	
    }
    m_i2c.stop();
	gotoxy(0,0);
}

void SSD1306::gotoxy(const uint8_t x, const uint8_t y) {
	m_x = x;
	m_y = y;
    m_i2c.start(OLED_I2C_ADDR);		
    m_i2c.byte(COMMAND);
    m_i2c.byte(SET_PAGE_START | y);
	m_i2c.byte(SET_COLUMN_ADDRESS);
    m_i2c.byte(x * 8);
	m_i2c.byte(SET_DISPLAY_START_LINE | 0x3F);
    m_i2c.stop();			
}

void SSD1306::fontSize(const uint8_t byte) {
	m_fontSize = byte;
}

void SSD1306::writeChar(const char c) {
	if (m_fontSize != 0) {		
		uint8_t tmp = 0;
		gotoxy(m_x, m_y);
		m_i2c.start(OLED_I2C_ADDR);
		m_i2c.byte(DATA);
		for (uint8_t count = 0; count < 8; count++) {
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = tmp & 0x0f;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			m_i2c.byte(tmp);
			m_i2c.byte(tmp);
		}
		m_i2c.stop();
		gotoxy(m_x, m_y + 1);
		m_i2c.start(OLED_I2C_ADDR);
		m_i2c.byte(DATA);
		for (uint8_t count = 0; count < 8; count++) {
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = (tmp & 0xf0)>>4;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			m_i2c.byte(tmp);
			m_i2c.byte(tmp);
		}
		m_i2c.stop();
		m_x += 2;
		m_y -= 1;
	} else {
		gotoxy(m_x, m_y);
		m_i2c.start(OLED_I2C_ADDR);
		m_i2c.byte(DATA);
		for (uint8_t count = 0; count < 8; count++){
			m_i2c.byte(pgm_read_byte(&font[(unsigned char)c][count]));
		}
		m_i2c.stop();
		m_x += 1;
	}
}

void SSD1306::writeStr(char *str){
    while (*str) {
        writeChar(*str++);
    }
}

void SSD1306::writeP(const char *Buffer, ...) {
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
		
    for(;;) {
		by = pgm_read_byte(Buffer++);
		if (by == 0) {
			break;
		}
            
		if (by == '%') {
            by = pgm_read_byte(Buffer++);
			if (isdigit(by) > 0) {                   
 				str_null_buffer[0] = by;
				str_null_buffer[1] = '\0';
				move = atoi(str_null_buffer);
                by = pgm_read_byte(Buffer++);
			}

			switch (by){
                case 's':
                    ptr = va_arg(ap,char *);
                    while(*ptr) {
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
					int b=0;
					while (str_buffer[b++] != 0){};
					b--;
					if (b<move) {
						move -=b;
						for (tmp = 0; tmp < move; tmp++) {
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
		}else{
			writeChar(by);	
		}
	}
	va_end(ap);
}
