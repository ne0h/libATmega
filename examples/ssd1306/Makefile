PROGRAMMER = "/dev/cu.usbserial-14310"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"i2c.d" -MT"i2c.o" -c -o "i2c.o" "../../i2c.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"ssd1306.d" -MT"ssd1306.o" -c -o "ssd1306.o" "../../ssd1306.cpp" -I../..
	avr-gcc -Wl,-Map,ssd1306.map -mmcu=atmega328p -o "ssd1306.elf" ./main.o ./i2c.o ./ssd1306.o
	avr-objdump -h -S ssd1306.elf  >"ssd1306.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex ssd1306.elf  "ssd1306.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ssd1306.elf  "ssd1306.eep"
	avr-size --format=avr --mcu=atmega328p ssd1306.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:ssd1306.hex:a

.PHONY: clean
clean:
	rm ssd1306.* main.o main.d || true
