#include "dht22.hpp"

bool DHT22::waitForHighUs(const uint8_t timeout) {
	for (uint8_t i = 0; i < timeout; i++) {
		if (DHT_PIN & (1<<DHT_INPUTPIN)) {
			return true;
		}
		_delay_us(1);
	}
	return false;
}

bool DHT22::waitForLowUs(const uint8_t timeout) {
	for (uint8_t i = 0; i < timeout; i++) {
		if (!(DHT_PIN & (1<<DHT_INPUTPIN))) {
			return true;
		}
		_delay_us(1);
	}
	return false;
}

Result DHT22::get(int16_t &temperature, int16_t &humidity) {
	uint8_t buf[5];
	memset(buf, 0, sizeof(buf));

	// set pin to OUTPUT
	DHT_DDR  |= (1<<DHT_INPUTPIN);
	DHT_PORT |= (1<<DHT_INPUTPIN);

	// PHASE 1: send start signal
	// set pin to LOW to send start signal
	DHT_PORT &= ~(1<<DHT_INPUTPIN);
	_delay_ms(10);

	// set pin to HIGH and wait for sensors's response
	// set pin to INPUT after waiting 40us
	DHT_PORT |= (1<<DHT_INPUTPIN);
	DHT_DDR &= ~(1<<DHT_INPUTPIN);
	_delay_us(40);

	// PHASE 2: wait until the sensor responds
	// ensure that the sensor pulls LOW
	// and wait 80us
	if ((DHT_PIN & (1<<DHT_INPUTPIN))) {
		return TimeoutError;
	}
	_delay_us(80);

	// ensure that the sensor pulls HIGH
	// and wait 80us
	if (!(DHT_PIN & (1<<DHT_INPUTPIN))) {
		return TimeoutError;
	}
	_delay_us(80);

	// PHASE 3: read 5 bytes from the sensor
	for (uint8_t bytes = 0; bytes < 5; bytes++) {
		uint8_t result = 0;
		for (uint8_t bits = 0; bits < 8; bits++) {
			if (!waitForHighUs(100)) { return TimeoutError; }
			_delay_us(30);
			if (DHT_PIN & (1<<DHT_INPUTPIN)) {
				result |= (1<<(7-bits));
			}
			if (!waitForLowUs(100)) { return TimeoutError; }
		}
		buf[bytes] = result;
	}

	temperature = buf[2] << 8 | buf[3];
	humidity = buf[0] << 8 | buf[1];

	// PHASE 4: calculate checksum and compare to last byte
	if ((buf[0] + buf[1] + buf[2] + buf[3]) != buf[4]) {
		return ChecksumError;
	}

	return Success;
}
