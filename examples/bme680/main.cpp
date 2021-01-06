#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <bme680.hpp>

int main() {

    AsyncUSART u;
    BME680 sensor;

    struct bme680_field_data data;
    while (1) {
        sensor.get(&data);
        char buf[128];
        snprintf(buf, 128, "{\n\t\"temp\": %.2f,\n\t\"humi\": %ld,\n\t\"pres\": %ld\n}\n", data.temperature / 100.0, data.humidity / 1000, data.pressure / 100);
        u.send(buf);
        _delay_ms(10000);
    }

    return 0;
}