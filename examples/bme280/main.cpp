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
        char buf[128];
        snprintf(
            buf,
            128,
            "{\n\t\"temperature\": %.2f,\n\t\"pressure\": %.0f,\n\t\"humidity\": %.0f\n}\n",
            data.temperature,
            data.pressure / 100,
            data.humidity
        );
        u.send(buf);
        _delay_ms(10000);
    }

    return 0;
}