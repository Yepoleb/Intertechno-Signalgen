.PHONY: all
all: build/main.hex

build/main.hex: build/main.elf
	avr-objcopy -j .text -j .data -O ihex build/main.elf build/main.hex

build/main.elf: main.c
	mkdir -p build
	avr-gcc -mmcu=atmega328p -O2 -o build/main.elf main.c

.PHONY: clean
clean:
	rm -f build/*

.PHONY: flash-new
flash-new: build/main.hex
	avrdude -v -c arduino -p atmega328p -P /dev/ttyUSB0 -b 115200 -Uflash:w:build/main.hex:i

.PHONY: flash-old
flash-old: build/main.hex
	avrdude -v -c arduino -p atmega328p -P /dev/ttyUSB0 -b 57600 -Uflash:w:build/main.hex:i
