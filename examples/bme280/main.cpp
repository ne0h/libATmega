#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <bme280.hpp>

int main() {

    AsyncUSART u;
    u.send("Starting ...\n");
    BME280 sensor;

    struct bme280_data data;
    while (1)
    {
        sensor.get(&data);
        u.sendDouble(data.temperature);      u.send(" °C\n");
        u.sendDouble(data.humidity);         u.send(" %\n");
        u.sendDouble(data.pressure / 100.0); u.send(" hPa\n");
        _delay_ms(10000);
    }

    return 0;
}