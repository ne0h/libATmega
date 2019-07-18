PROGRAMMER = "/dev/cu.wchusbserial1420"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"dht22.d" -MT"dht22.o" -c -o "dht22.o" "../../dht22.cpp"
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"i2c.d" -MT"i2c.o" -c -o "i2c.o" "../../i2c.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"ssd1306.d" -MT"ssd1306.o" -c -o "ssd1306.o" "../../ssd1306.cpp"
	avr-gcc -Wl,-Map,thermometer.map -mmcu=atmega328p -o "thermometer.elf" ./main.o ./dht22.o ./i2c.o ./ssd1306.o
	avr-objdump -h -S thermometer.elf  >"thermometer.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex thermometer.elf "thermometer.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex thermometer.elf  "thermometer.eep"
	avr-size --format=avr --mcu=atmega328p thermometer.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:thermometer.hex:a

.PHONY: clean
clean:
	rm -f thermometer.* dht22.o dht22.d main.o main.d ssd1306.o ssd1306.d
