# CO321 Embedded Systems Lab

## Programming AVR Microcontrollers (ATmega328P in C)

This repository contains lab materials and example source code for CO321 - Embedded Systems (ATmega328P). The goal is to program the AVR directly in C (without Arduino IDE abstractions).

**Contents:** hardware notes, example source files, build rules and upload instructions.

## Prerequisites

Install the development tools on Debian/Ubuntu:

```bash
sudo apt update
sudo apt install -y git make gcc-avr avr-libc binutils-avr avrdude
```

Tools and purpose:
- `gcc-avr`: AVR C compiler
- `avr-libc`: AVR C runtime and headers
- `binutils-avr` / `avr-objcopy`: object tools and hex conversion
- `avrdude`: upload HEX to microcontroller
- `make`: build automation

## Quick start (local)

1. Clone the repository (if needed):

```bash
git clone https://github.com/Ganathipan/CO321_labs.git
cd CO321_labs
```

2. Build an example (if a `Makefile` exists in a lab folder):

```bash
cd lab00
make
```

If there is no Makefile, a minimal compile + hex conversion example:

```bash
avr-gcc -mmcu=atmega328p -Os -DF_CPU=16000000UL -std=gnu11 -c -o led.o led.c
avr-gcc -mmcu=atmega328p -Os -o led.elf led.o
avr-objcopy -O ihex -R .eeprom led.elf led.hex
```

## Uploading to the MCU

Adjust the programmer and port to your hardware. Example using `usbasp`:

```bash
avrdude -c usbasp -p m328p -U flash:w:led.hex
```

Or for an Arduino as ISP on `/dev/ttyACM0`:

```bash
avrdude -c arduino -P /dev/ttyACM0 -b 115200 -p m328p -U flash:w:led.hex
```

## License & Contact

Check course materials for license and redistribution rules. For repository issues, contact the repository owner.
