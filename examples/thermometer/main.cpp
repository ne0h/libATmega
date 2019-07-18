#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <ssd1306.hpp>
#include <dht22.hpp>

AsyncUSART u;

int main() {
	SSD1306 oled;

	oled.gotoxy(2, 1);
	oled.write("Thermometer");
	oled.gotoxy(2, 2);
	oled.write("-----------");

	oled.gotoxy(1, 5);
    oled.write("Tmp:");
    oled.gotoxy(1, 6);
    oled.write("Hum:");


	int16_t temperature, humidity;

	while (1) {
		auto result = DHT22::get(temperature, humidity);
		
		// send via USART
		u.send("Result: "); u.sendAsDecChar(result); u.send('\n');
		u.send("Tmp: "); u.sendAsDecChar(temperature); u.send('\n');
		u.send("Hum: "); u.sendAsDecChar(humidity); u.send('\n');

		// print on OLED
		oled.gotoxy(6, 5);
		oled.write("%i.%iC", temperature / 10, temperature % 10);
		oled.gotoxy(6, 6);
		oled.write("%i.%i", humidity / 10, humidity % 10);
				
		_delay_ms(1000);
	}

	return 0;
}
