#include <avr/io.h>
#include <util/delay.h>

#include <usart.hpp>
#include <bme680.hpp>

int main() {

    AsyncUSART u;
    BME680 sensor(false);

    // read delay until the measurement is ready from sensor
    const uint16_t meas_delay = sensor.get_meas_periode();
    char buf[32];
    snprintf(buf, 32, "Meas delay: %hu ms\n", meas_delay);
    u.send(buf);

    struct bme680_field_data data;
    while (1) {
        sensor.get(&data);
        char buf[128];
        snprintf(
            buf,
            128,
            "{\n\t\"status\": %d,\n\t\"temperature\": %.2f,\n\t\"pressure\": %ld,\n\t\"humidity\": %ld\n}\n",
            data.status,
            data.temperature / 100.0,
            data.pressure / 100,
            data.humidity / 1000
        );
        u.send(buf);
        _delay_ms(10000);
    }

    return 0;
}