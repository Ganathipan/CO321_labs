# Simple AVR-GCC Makefile for ATmega328P lab exercises
# Edit PORT before flashing. Windows examples: COM3, COM4. Linux examples: /dev/ttyUSB0, /dev/ttyACM0

MCU        = atmega328p
F_CPU      = 16000000UL
PROGRAMMER = arduino
PORT       = COM3
BAUD       = 115200
TARGET     = main

CC      = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -std=gnu99 -Wall -Wextra -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$(TARGET).hex:i

clean:
	rm -f *.elf *.hex *.o

.PHONY: all flash clean
