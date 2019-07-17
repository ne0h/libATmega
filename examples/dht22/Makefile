PROGRAMMER = "/dev/cu.wchusbserial1420"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"dht22.d" -MT"dht22.o" -c -o "dht22.o" "../../dht22.cpp"
	avr-gcc -Wl,-Map,dht.map -mmcu=atmega328p -o "dht.elf" ./main.o ./dht22.o
	avr-objdump -h -S dht.elf  >"dht.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex dht.elf "dht.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex dht.elf  "dht.eep"
	avr-size --format=avr --mcu=atmega328p dht.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:dht.hex:a

.PHONY: clean
clean:
	rm -f dht.* dht22.o dht22.d main.o main.d
