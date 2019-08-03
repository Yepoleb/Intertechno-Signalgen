# Intertechno Signal Generator

Generates the signal required for switching Intertechno remote controlled
power sockets using an Arduino Uno or Nano. Does not use any parts of the
official Arduino SDK, but relies on a locally installed version of avr-gcc,
avr-libc and avrdude instead.

## Building

```
sudo apt install gcc-avr avr-libc avrdude
make all
```

## Flashing

`make flash-new` for the new bootloader or `make flash-old` for the old one.

## License

CC0
