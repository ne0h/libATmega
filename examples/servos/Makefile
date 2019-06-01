PROGRAMMER = "/dev/cu.wchusbserial1420"

default: clean build upload

.PHONY: build
build:
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wl,-Map,servos.map -mmcu=atmega328p -o "servos.elf" ./main.o
	avr-objdump -h -S servos.elf  >"servos.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex servos.elf  "servos.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex servos.elf "servos.eep"
	avr-size --format=avr --mcu=atmega328p servos.elf

.PHONY: upload
upload:
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:servos.hex:a

.PHONY: clean
clean:
	rm servos.* main.o main.d || true
