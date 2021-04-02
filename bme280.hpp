#pragma once

#include <string.h>
#include <util/delay.h>
#include "spi.hpp"
#include "3rdparty/BME280_driver/bme280.h"
#include "3rdparty/BME280_driver/bme280_defs.h"

class BME280 {

private:    
    struct bme280_dev m_sensor;
    uint32_t m_req_delay;

public:
    BME280();

    int8_t get(struct bme280_data *data);
};
