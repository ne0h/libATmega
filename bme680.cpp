#include "bme680.hpp"

static volatile uint8_t *CS_DDR = &DDRB;
static volatile uint8_t *CS_PORT = &PORTB;
static const uint8_t CS_PIN = PB2;

static int8_t spi_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
static int8_t spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
static void   delay_ms (uint32_t delay);

static SPI spi;

// init is taken from https://github.com/BoschSensortec/BME680_driver#example-for-spi-4-wire
BME680::BME680(bool enable_gas_sensor) {
    // Set CS pin
    *CS_DDR |= (1 << CS_PIN);

    m_gas_sensor.dev_id = 0;
    m_gas_sensor.intf = BME680_SPI_INTF;
    m_gas_sensor.read = spi_read;
    m_gas_sensor.write = spi_write;
    m_gas_sensor.delay_ms = delay_ms;
    /* amb_temp can be set to 25 prior to configuring the gas sensor 
     * or by performing a few temperature readings without operating the gas sensor.
     */
    //m_gas_sensor.amb_temp = 25;
    bme680_init(&m_gas_sensor);

    m_gas_sensor.tph_sett.os_hum = BME680_OS_2X;
    m_gas_sensor.tph_sett.os_pres = BME680_OS_4X;
    m_gas_sensor.tph_sett.os_temp = BME680_OS_8X;
    m_gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    m_gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS ? enable_gas_sensor : BME680_DISABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    m_gas_sensor.gas_sett.heatr_temp = 320 ? enable_gas_sensor : 0; /* degree Celsius */
    m_gas_sensor.gas_sett.heatr_dur  = 240 ? enable_gas_sensor : 0; /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    m_gas_sensor.power_mode = BME680_FORCED_MODE;

    /* Set the required sensor settings needed */
    const uint8_t set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    bme680_set_sensor_settings(set_required_settings, &m_gas_sensor);

    /* Set the power mode */
    bme680_set_sensor_mode(&m_gas_sensor);

    bme680_get_profile_dur(&m_meas_period, &m_gas_sensor);
}

uint16_t BME680::get_meas_periode() {
    uint16_t meas_period;
    bme680_get_profile_dur(&meas_period, &m_gas_sensor);
    return meas_period;
}

int8_t BME680::get(struct bme680_field_data *data) {
    memset(data, 0, sizeof(struct bme680_field_data));

    // Delay till the measurement is ready
    delay_ms(m_meas_period);

    // read the data
    const int8_t res = bme680_get_sensor_data(data, &m_gas_sensor);

    // trigger the next measurement
    bme680_set_sensor_mode(&m_gas_sensor);

    data->status &= BME680_SPI_WR_MSK;
    return res;
}

static int8_t spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    spi.start(CS_PORT, CS_PIN);
    spi.write(reg_addr);
    for (uint16_t i = 0; i < len; i++)
    {
        data[i] = spi.read();
    }
    spi.stop(CS_PORT, CS_PIN);
    return 0;
}

static int8_t spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    spi.start(CS_PORT, CS_PIN);
    spi.write(reg_addr);
    for (uint16_t i = 0; i < len; i++)
    {
        spi.write(data[i]);
    }
    spi.stop(CS_PORT, CS_PIN);
    return 0;
}

static void delay_ms(uint32_t period) {
    while (period--)
    {
        _delay_ms(1);
    }
}
