#include <ssd1306.hpp>

int main() {

    SSD1306 oled;

    oled.gotoxy(0, 1);
    oled.write(" Max ");

    while (1) {
        for (uint8_t i = 0; i < 100; i++) {
            oled.gotoxy(0, 3);
            oled.write("%2i", i);
            _delay_ms(1000);
        }
    }

    return 0;
}