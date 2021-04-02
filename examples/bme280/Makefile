PROGRAMMER = "/dev/cu.usbserial-110"
#TARGET="atmega328p"
TARGET="atmega2560"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=${TARGET} -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=${TARGET} -DF_CPU=16000000UL -MMD -MP -MF"bme280.d" -MT"bme280.o" -c -o "bme280.o" "../../bme280.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=${TARGET} -DF_CPU=16000000UL -MMD -MP -MF"bme280_driver.d" -MT"bme280_driver.o" -c -o "bme280_driver.o" "../../3rdparty/BME280_driver/bme280.c" -I../..
	avr-gcc -Wl,-Map,bme280.map -mmcu=${TARGET} -o "bme280.elf" -Wl,-u,vfprintf -lprintf_flt -lm ./main.o ./bme280.o ./bme280_driver.o
	avr-objdump -h -S bme280.elf  >"bme280.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex bme280.elf  "bme280.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex bme280.elf  "bme280.eep"
	avr-size --format=avr --mcu=${TARGET} bme280.elf

.PHONY: upload
upload:
ifeq (${TARGET}, "atmega328p")
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:bme280.hex:a
else ifeq (${TARGET}, "atmega2560")
	avrdude -pm2560 -cwiring -P${PROGRAMMER} -Uflash:w:bme280.hex:a -D
endif

.PHONY: clean
clean:
	rm -f bme280.* bme280_driver.* i2c.* main.o main.d
