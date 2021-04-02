#pragma once

#include <string.h>
#include <util/delay.h>
#include "spi.hpp"
#include "3rdparty/BME680_driver/bme680.h"
#include "3rdparty/BME680_driver/bme680_defs.h"

class BME680
{

private:
    struct bme680_dev m_gas_sensor;
    uint16_t m_meas_period;

public:
    BME680(bool enable_gas_sensor);

    int8_t get(struct bme680_field_data *data);

    uint16_t get_meas_periode();
};
