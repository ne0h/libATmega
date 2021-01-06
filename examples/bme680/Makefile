PROGRAMMER = "/dev/cu.usbserial-14110"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"i2c.d" -MT"i2c.o" -c -o "i2c.o" "../../i2c.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"bme680.d" -MT"bme680.o" -c -o "bme680.o" "../../bme680.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"bme680_driver.d" -MT"bme680_driver.o" -c -o "bme680_driver.o" "../../3rdparty/BME680_driver/bme680.c" -I../..
	avr-gcc -Wl,-Map,bme680.map -mmcu=atmega328p -o "bme680.elf" -Wl,-u,vfprintf -lprintf_flt -lm ./main.o ./i2c.o ./bme680.o ./bme680_driver.o
	avr-objdump -h -S bme680.elf  >"bme680.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex bme680.elf  "bme680.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex bme680.elf  "bme680.eep"
	avr-size --format=avr --mcu=atmega328p bme680.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:bme680.hex:a

.PHONY: clean
clean:
	rm -f bme680* spi.* main.o main.d
