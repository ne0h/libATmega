PROGRAMMER = "/dev/cu.usbserial-A906R2H4"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wl,-Map,log.map -mmcu=atmega328p -o "log.elf"  ./main.o
	avr-objdump -h -S log.elf  >"log.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex log.elf  "log.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex log.elf  "log.eep"
	avr-size --format=avr --mcu=atmega328p log.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:log.hex:a

.PHONY: clean
clean:
	rm log.* main.o main.d || true
