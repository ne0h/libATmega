#pragma once

#include <avr/io.h>

enum ADCDivisionFactor {
    ADCDIVISIONFACTOR_2,
    ADCDIVISIONFACTOR_4,
    ADCDIVISIONFACTOR_8,
    ADCDIVISIONFACTOR_16,
    ADCDIVISIONFACTOR_32,
    ADCDIVISIONFACTOR_64,
    ADCDIVISIONFACTOR_128
};

class AnalogDigitalConverter {

public:
    AnalogDigitalConverter(const ADCDivisionFactor df) {
        ADMUX  |= (1<<REFS1) | (1<<REFS0);
        switch (df) {
        case ADCDIVISIONFACTOR_2:
            ADCSRA |= (1<<ADPS0);
            break;
        case ADCDIVISIONFACTOR_4:
            ADCSRA |= (1<<ADPS1);
            break;
        case ADCDIVISIONFACTOR_8:
            ADCSRA |= (1<<ADPS1) | (1<<ADPS0);
            break;
        case ADCDIVISIONFACTOR_16:
            ADCSRA |= (1<<ADPS2);
            break;
        case ADCDIVISIONFACTOR_32:
            ADCSRA |= (1<<ADPS2) | (1<<ADPS0);
            break;
        case ADCDIVISIONFACTOR_64:
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1);
            break;
        case ADCDIVISIONFACTOR_128:
            ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
            break;
        }
        ADCSRA |= (1<<ADEN); 
    }

    uint16_t read(const uint8_t channel) {
        ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
        ADCSRA |= (1<<ADSC);
        while (ADCSRA & (1<<ADSC));
        return ADC;
    }

};
