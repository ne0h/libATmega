#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <adc.hpp>

const float ADC_FACTOR = 4.7 / 1023.f * 1000;

int main() {
	USART u;
	AnalogDigitalConverter adc(ADCDIVISIONFACTOR_128);

	DDRD |= (1<<PD2);
	PORTD |= (1<<PD2);

	while (1) {
		const uint16_t value = adc.read(0);
		u.sendAsDecChar((uint16_t)(value * ADC_FACTOR));
		u.send("mV\n");
		_delay_ms(100);
	}

	return 0;
}
