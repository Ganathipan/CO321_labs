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

---

## Build and Upload Process

1. Verify Installation:

Check whether the tools are installed correctly:

```bash
avr-gcc --version
avr-objcopy --version
avrdude -v
make --version
git --version
```

If all commands display version information, the setup is ready.

2. Clone the repository (if needed):

```bash
git clone https://github.com/Ganathipan/CO321_labs.git
cd CO321_labs
```
3. Compile the C Program

```bash
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o led led.c
```

Command explanation:

| Part | Meaning |
|---|---|
| `avr-gcc` | AVR C compiler |
| `-Os` | Optimize code size |
| `-DF_CPU=16000000UL` | Define CPU frequency as 16 MHz |
| `-mmcu=atmega328p` | Select ATmega328P microcontroller |
| `-o led` | Output file name |
| `led.c` | Source file |

4. Generate the HEX File

```bash
avr-objcopy -O ihex -R .eeprom led led.hex
```

This creates a HEX file that can be uploaded to the microcontroller.

5. Connect the Arduino Uno

Connect the Arduino Uno to your computer using a USB cable.

If you are using WSL2, run `usbipd` on Windows PowerShell, not inside Linux/WSL.

If `usbipd` is not recognized in PowerShell, install `usbipd-win` on Windows first:

```powershell
winget install --id dorssel.usbipd-win -e
```

Then list and attach the board from Windows PowerShell:

```powershell
usbipd list
usbipd bind --busid <BUSID>
usbipd attach --wsl --busid <BUSID>
```

After attaching, check inside WSL with:

```bash
ls /dev/ttyACM*
ls /dev/ttyUSB*
```

Common Arduino serial ports are:

```text
/dev/ttyACM0
/dev/ttyUSB0
```

Use the detected port when uploading the program.

```bash
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:led.hex
```

Command explanation:

| Part | Meaning |
|---|---|
| `avrdude` | Upload tool |
| `-c arduino` | Use Arduino programmer |
| `-p ATMEGA328P` | Target microcontroller |
| `-P /dev/ttyACM0` | Serial port |
| `-b 115200` | Baud rate |
| `-U flash:w:led.hex` | Write `led.hex` to flash memory |

6. Fix Serial Port Permission Issues

If uploading fails due to permission errors, add your user to the `dialout` group:

```bash
sudo usermod -a -G dialout $USER
```

Then log out and log back in.

For a temporary fix:

```bash
sudo chmod a+rw /dev/ttyACM0
```

Replace `/dev/ttyACM0` with your actual serial port if needed.

---

## Makefile Usage

If a `Makefile` is included, the build process can be simplified.

### Build

```bash
make
```

### Upload

```bash
make flash
```

### Clean Generated Files

```bash
make clean
```

This avoids typing long compile and upload commands repeatedly.

---

## ATmega328P Port Types

The ATmega328P has three main digital input/output ports:

| Port | Pin Range | Arduino Pin Mapping | Main Usage |
|---|---|---|---|
| `PORTB` | PB0 – PB7 | Digital pins 8 – 13 | Digital I/O, SPI, built-in LED |
| `PORTC` | PC0 – PC6 | Analog pins A0 – A5, Reset | Analog input, I2C, reset |
| `PORTD` | PD0 – PD7 | Digital pins 0 – 7 | Digital I/O, UART, interrupts, PWM |

Each port is controlled using three main registers:

```text
DDRx   → Data Direction Register
PORTx  → Output Register / Pull-up Control
PINx   → Input Register
```

Here, `x` represents the port letter:

```text
B, C, or D
```

So the actual registers are:

```text
DDRB, PORTB, PINB
DDRC, PORTC, PINC
DDRD, PORTD, PIND
```

## PORTB Details

`PORTB` is mainly connected to Arduino digital pins **8 to 13**.

| Arduino Uno Pin | ATmega328P Pin | Port Bit | Special Function |
|---|---|---|---|
| Digital 8 | PB0 | PORTB0 | General digital I/O |
| Digital 9 | PB1 | PORTB1 | PWM |
| Digital 10 | PB2 | PORTB2 | PWM / SPI SS |
| Digital 11 | PB3 | PORTB3 | PWM / SPI MOSI |
| Digital 12 | PB4 | PORTB4 | SPI MISO |
| Digital 13 | PB5 | PORTB5 | SPI SCK / Built-in LED |
| Crystal Pin | PB6 | PORTB6 | External crystal |
| Crystal Pin | PB7 | PORTB7 | External crystal |

Example:

```c
DDRB |= (1 << PB5);      // Set Arduino pin 13 as output
PORTB |= (1 << PB5);     // Set pin 13 HIGH
PORTB &= ~(1 << PB5);    // Set pin 13 LOW
```

## PORTC Details

`PORTC` is mainly connected to Arduino analog pins **A0 to A5**.

| Arduino Uno Pin | ATmega328P Pin | Port Bit | Special Function |
|---|---|---|---|
| Analog A0 | PC0 | PORTC0 | ADC0 |
| Analog A1 | PC1 | PORTC1 | ADC1 |
| Analog A2 | PC2 | PORTC2 | ADC2 |
| Analog A3 | PC3 | PORTC3 | ADC3 |
| Analog A4 | PC4 | PORTC4 | ADC4 / I2C SDA |
| Analog A5 | PC5 | PORTC5 | ADC5 / I2C SCL |
| Reset | PC6 | RESET | Reset pin |

Although A0 to A5 are called analog pins in Arduino, they can also be used as digital I/O pins in AVR C.

Example:

```c
DDRC |= (1 << PC0);      // Set A0 as digital output
PORTC |= (1 << PC0);     // Set A0 HIGH
PORTC &= ~(1 << PC0);    // Set A0 LOW
```

## PORTD Details

`PORTD` is mainly connected to Arduino digital pins **0 to 7**.

| Arduino Uno Pin | ATmega328P Pin | Port Bit | Special Function |
|---|---|---|---|
| Digital 0 | PD0 | PORTD0 | UART RX |
| Digital 1 | PD1 | PORTD1 | UART TX |
| Digital 2 | PD2 | PORTD2 | External interrupt INT0 |
| Digital 3 | PD3 | PORTD3 | PWM / External interrupt INT1 |
| Digital 4 | PD4 | PORTD4 | General digital I/O |
| Digital 5 | PD5 | PORTD5 | PWM |
| Digital 6 | PD6 | PORTD6 | PWM |
| Digital 7 | PD7 | PORTD7 | General digital I/O |

Example:

```c
DDRD |= (1 << PD6);      // Set Arduino pin 6 as output
PORTD |= (1 << PD6);     // Set pin 6 HIGH
PORTD &= ~(1 << PD6);    // Set pin 6 LOW
```

## AVR GPIO Register Details

### 1. DDRx – Data Direction Register

The `DDRx` register controls whether each pin in a port is an input or an output.

| Bit Value | Meaning |
|---|---|
| `1` | Output |
| `0` | Input |

Example:

```c
DDRB |= (1 << PB5);
```

This sets **PB5** as an output pin.

Since PB5 is connected to Arduino digital pin 13, this is equivalent to:

```c
pinMode(13, OUTPUT);
```

### 2. PORTx – Port Output Register

When a pin is configured as an output, the `PORTx` register controls whether the output is HIGH or LOW.

| Bit Value | Meaning |
|---|---|
| `1` | HIGH |
| `0` | LOW |

Example:

```c
PORTB |= (1 << PB5);
```

This sets PB5 HIGH.

Equivalent Arduino code:

```c
digitalWrite(13, HIGH);
```

To set PB5 LOW:

```c
PORTB &= ~(1 << PB5);
```

Equivalent Arduino code:

```c
digitalWrite(13, LOW);
```

### 3. PINx – Port Input Register

The `PINx` register is used to read the current input state of a pin.

Example:

```c
if (PINB & (1 << PB5))
{
    // PB5 is HIGH
}
else
{
    // PB5 is LOW
}
```

This is similar to:

```c
digitalRead(13);
```

## Arduino Functions vs AVR Register Programming

| Arduino Function | AVR Register Equivalent |
|---|---|
| `pinMode(13, OUTPUT)` | `DDRB |= (1 << PB5);` |
| `digitalWrite(13, HIGH)` | `PORTB |= (1 << PB5);` |
| `digitalWrite(13, LOW)` | `PORTB &= ~(1 << PB5);` |
| `digitalRead(13)` | `PINB & (1 << PB5)` |

## Common Bitwise Operations in AVR C

AVR programming uses bitwise operations to control individual pins without affecting the other pins in the same port.

| Operation | Purpose | Example |
|---|---|---|
| `|=` | Set a bit to 1 | `PORTB |= (1 << PB5);` |
| `&= ~` | Clear a bit to 0 | `PORTB &= ~(1 << PB5);` |
| `&` | Check whether a bit is 1 | `PINB & (1 << PB5)` |
| `^=` | Toggle a bit | `PORTB ^= (1 << PB5);` |

Example for toggling an LED:

```c
PORTB ^= (1 << PB5);
```

This changes the LED state:

```text
ON  → OFF
OFF → ON
```

## Special Pin Functions

Some ATmega328P pins have extra hardware functions.

| Arduino Pin | AVR Pin | Special Function |
|---|---|---|
| Digital 0 | PD0 | UART RX |
| Digital 1 | PD1 | UART TX |
| Digital 2 | PD2 | External interrupt INT0 |
| Digital 3 | PD3 | PWM / External interrupt INT1 |
| Digital 5 | PD5 | PWM |
| Digital 6 | PD6 | PWM |
| Digital 9 | PB1 | PWM |
| Digital 10 | PB2 | PWM / SPI SS |
| Digital 11 | PB3 | SPI MOSI |
| Digital 12 | PB4 | SPI MISO |
| Digital 13 | PB5 | SPI SCK |
| Analog A4 | PC4 | I2C SDA |
| Analog A5 | PC5 | I2C SCL |

Important SPI mapping:

```text
MOSI → PB3 → Arduino digital pin 11
MISO → PB4 → Arduino digital pin 12
SCK  → PB5 → Arduino digital pin 13
```

Digital pins **11, 12, and 13** are also connected to the ICSP header on the Arduino Uno. Avoid connecting heavy loads to these pins when using SPI or ICSP programming.

---

## License & Contact

Check course materials for license and redistribution rules. For repository issues, contact the repository owner.
