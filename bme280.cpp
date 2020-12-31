#include "bme280.hpp"

static volatile uint8_t *CS_DDR  = &DDRB;
static volatile uint8_t *CS_PORT = &PORTB;
static const    uint8_t  CS_PIN  = PB2;

static void   delay_us (uint32_t period, void *intf_ptr);
static int8_t spi_read (uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
static int8_t spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

static SPI spi;

BME280::BME280() {

    // Set CS pin
    *CS_DDR  |= (1<<CS_PIN);

    // Sensor_0 interface over SPI with native chip select line
    uint8_t dev_addr = 0;

    m_sensor.intf_ptr = &dev_addr;
    m_sensor.intf = BME280_SPI_INTF;
    m_sensor.read = spi_read;
    m_sensor.write = spi_write;
    m_sensor.delay_us = delay_us;
    bme280_init(&m_sensor);

    m_sensor.settings.osr_h  = BME280_OVERSAMPLING_1X;
	m_sensor.settings.osr_p  = BME280_OVERSAMPLING_1X;
	m_sensor.settings.osr_t  = BME280_OVERSAMPLING_1X;
	m_sensor.settings.filter = BME280_FILTER_COEFF_OFF;
	const uint8_t settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
	bme280_set_sensor_settings(settings_sel, &m_sensor);

	// calculate minimum delay required between consecutive measurments
	m_req_delay = bme280_cal_meas_delay(&m_sensor.settings);
}

int8_t BME280::get(struct bme280_data *data) {
    memset(data, 0, sizeof(struct bme280_data));
    bme280_set_sensor_mode(BME280_FORCED_MODE, &m_sensor);
    delay_us(m_req_delay * 1000, NULL);
    bme280_get_sensor_data(BME280_ALL, data, &m_sensor);
    return 0;
}

static void delay_us(uint32_t period, void *intf_ptr) {
    while (period--) {
        _delay_us(1);
    }
}

static int8_t spi_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    spi.start(CS_PORT, CS_PIN);
    spi.write(reg_addr);
    for (uint32_t i = 0; i < len; i++) {
        data[i] = spi.read();
    }
    spi.stop(CS_PORT, CS_PIN);
    return 0;
}

static int8_t spi_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    spi.start(CS_PORT, CS_PIN);
    spi.write(reg_addr);
    for (uint32_t i = 0; i < len; i++) {
        spi.write(data[i]);
    }
    spi.stop(CS_PORT, CS_PIN);
    return 0;
}
