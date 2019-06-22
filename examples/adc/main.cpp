#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <adc.hpp>

const float ADC_FACTOR = 5.f / 1023.f * 1000;

int main() {
	USART u;
	AnalogDigitalConverter adc(ADCDIVISIONFACTOR_128);

	while (1) {
		const uint16_t value = adc.read(0);
		u.sendAsDecChar((uint16_t)(value * ADC_FACTOR));
		u.send("mV\n");
		_delay_ms(100);
	}

	return 0;
}
