/*
 * CO321 Embedded Systems - Lab 2: Interrupts
 * Exercise 4: Binary number input using two external interrupt buttons
 *
 * Task:
 * Connect two push buttons A and B to two external interrupt pins.
 * Connect 6 LEDs to a GPIO port.
 * - Press A to input binary 0.
 * - Press B to input binary 1.
 * Display the updated 6-bit number on LEDs every time a button is pressed.
 *
 * Wiring assumption:
 * - Button A is connected between PD2/INT0 and GND.
 * - Button B is connected between PD3/INT1 and GND.
 * - Internal pull-ups are enabled.
 * - Pressed = LOW.
 * - Press event = falling edge.
 * - Six LEDs are connected to PB0-PB5.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define BUTTON_A_PIN    PD2        // INT0, input bit 0
#define BUTTON_B_PIN    PD3        // INT1, input bit 1
#define LED_MASK        0x3F       // PB0-PB5
#define DEBOUNCE_MS     25

volatile uint8_t number = 0;

static void display_number(uint8_t value)
{
    PORTB = (PORTB & ~LED_MASK) | (value & LED_MASK);
}

static void append_bit(uint8_t bit_value)
{
    // Shift previous bits left and insert the new bit at LSB.
    // Mask with 0x3F so only the latest 6 bits are displayed.
    number = ((number << 1) | (bit_value & 0x01)) & LED_MASK;
    display_number(number);
}

ISR(INT0_vect)
{
    // Button A inputs 0.
    // Disable INT0 during debounce to reduce repeated bounce interrupts.
    EIMSK &= ~(1 << INT0);
    _delay_ms(DEBOUNCE_MS);

    // Confirm A is still pressed.
    if (!(PIND & (1 << BUTTON_A_PIN)))
    {
        append_bit(0);
    }

    // Clear any pending INT0 flag caused by bounce, then re-enable INT0.
    EIFR |= (1 << INTF0);
    EIMSK |= (1 << INT0);
}

ISR(INT1_vect)
{
    // Button B inputs 1.
    EIMSK &= ~(1 << INT1);
    _delay_ms(DEBOUNCE_MS);

    // Confirm B is still pressed.
    if (!(PIND & (1 << BUTTON_B_PIN)))
    {
        append_bit(1);
    }

    // Clear any pending INT1 flag caused by bounce, then re-enable INT1.
    EIFR |= (1 << INTF1);
    EIMSK |= (1 << INT1);
}

int main(void)
{
    // Configure PB0-PB5 as output LEDs.
    DDRB |= LED_MASK;
    display_number(0);

    // Configure PD2 and PD3 as inputs.
    DDRD &= ~((1 << BUTTON_A_PIN) | (1 << BUTTON_B_PIN));

    // Enable internal pull-up resistors on PD2 and PD3.
    PORTD |= (1 << BUTTON_A_PIN) | (1 << BUTTON_B_PIN);

    // Configure INT0 and INT1 to trigger on falling edge.
    // Falling edge = HIGH to LOW = button press with pull-up wiring.
    EICRA |= (1 << ISC01);  // INT0 falling edge: ISC01=1, ISC00=0
    EICRA &= ~(1 << ISC00);

    EICRA |= (1 << ISC11);  // INT1 falling edge: ISC11=1, ISC10=0
    EICRA &= ~(1 << ISC10);

    // Enable external interrupts INT0 and INT1.
    EIMSK |= (1 << INT0) | (1 << INT1);

    // Enable global interrupts.
    sei();

    while (1)
    {
        // Main loop is free. Button input is handled by interrupts.
    }

    return 0;
}
