#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "common.hpp"

#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN PIND
#define DHT_INPUTPIN PD6

class DHT22 {

private:
	static bool waitForHighUs(const uint8_t timeout);
	static bool waitForLowUs(const uint8_t timeout);

public:
	static Result get(int16_t &temperature, int16_t &humidity);

};
