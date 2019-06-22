PROGRAMMER = "/dev/cu.wchusbserial1420"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wl,-Map,adc.map -mmcu=atmega328p -o "adc.elf"  ./main.o
	avr-objdump -h -S adc.elf  >"adc.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex adc.elf "adc.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex adc.elf  "adc.eep"
	avr-size --format=avr --mcu=atmega328p adc.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:adc.hex:a

.PHONY: clean
clean:
	rm adc.* main.o main.d || true
