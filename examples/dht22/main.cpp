#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <dht22.hpp>

AsyncUSART u;

int main() {
	int16_t temperature, humidity;

	while (1) {
		auto result = DHT22::get(temperature, humidity);
		u.send("Result: "); u.sendAsDecChar(result); u.send('\n');

		u.send("Tmp: "); u.sendAsDecChar(temperature); u.send('\n');
		u.send("Hum: "); u.sendAsDecChar(humidity); u.send('\n');
				
		_delay_ms(1000);
	}

	return 0;
}
