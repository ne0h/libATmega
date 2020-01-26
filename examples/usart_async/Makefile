PROGRAMMER = "/dev/cu.usbserial-A906R2H4"
TARGET = "atmega2560"

default: clean build upload

.PHONY: build
build:
ifeq (${TARGET}, "atmega328p")
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wl,-Map,usart_async.map -mmcu=atmega328p -o "usart_async.elf" ./main.o
else ifeq (${TARGET}, "atmega2560")
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "main.cpp" -I../..
	avr-gcc -Wl,-Map,usart_async.map -mmcu=atmega2560 -o "usart_async.elf" ./main.o
else
	@echo "[ERROR] Unknown TARGET '${TARGET}'"
	exit 1
endif
	avr-objdump -h -S usart_async.elf  >"usart_async.lss"
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex usart_async.elf  "usart_async.hex"
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex usart_async.elf  "usart_async.eep"
ifeq (${TARGET}, "atmega328p")	
	avr-size --format=avr --mcu=atmega328p usart_async.elf
else ifeq (${TARGET}, "atmega2560")
	avr-size --format=avr --mcu=atmega2560 usart_async.elf
endif

.PHONY: upload
upload:
ifeq (${TARGET}, "atmega328p")
	avrdude -pm328p -carduino -P${PROGRAMMER} -b57600 -Uflash:w:usart_async.hex:a
else ifeq (${TARGET}, "atmega2560")
	avrdude -pm2560 -carduino -P${PROGRAMMER} -b57600 -Uflash:w:usart_async.hex:a
endif

.PHONY: clean
clean:
	rm usart_async.* main.o main.d || true
