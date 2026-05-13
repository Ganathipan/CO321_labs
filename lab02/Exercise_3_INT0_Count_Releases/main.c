/*
 * CO321 Embedded Systems - Lab 2: Interrupts
 * Exercise 3: Count button releases using INT0 and display count on six LEDs
 *
 * Task:
 * Extend Exercise 2 so that the number of times the push button was released
 * is displayed as a binary number on LEDs connected to PORTB.
 *
 * Wiring assumption:
 * - Button is connected between PD2/INT0 and GND.
 * - Internal pull-up is enabled on PD2.
 * - Pressed = LOW, released = HIGH.
 * - Release event = rising edge.
 * - Six LEDs are connected to PB0-PB5.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define BUTTON_INT0_PIN PD2
#define LED_MASK        0x1F        // PB0-PB5
#define DEBOUNCE_MS     50

volatile uint8_t release_count = 0;

static void display_count(uint8_t count)
{
    PORTB = (PORTB & ~LED_MASK) | (count & LED_MASK);
}

ISR(INT0_vect)
{
    // Debounce for lab demonstration.
    _delay_ms(DEBOUNCE_MS);

    // Confirm button has been released.
    if (PIND & (1 << BUTTON_INT0_PIN))
    {
        release_count = (release_count + 1) & LED_MASK;  // 0 to 63 range
        display_count(release_count);
    }
}

int main(void)
{
    // Configure PB0-PB5 as output LEDs.
    DDRB |= LED_MASK;
    display_count(0);

    // Configure PD2/INT0 as input and enable pull-up.
    DDRD &= ~(1 << BUTTON_INT0_PIN);
    PORTD |= (1 << BUTTON_INT0_PIN);

    // INT0 rising edge trigger: button release.
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // Enable INT0.
    EIMSK |= (1 << INT0);

    // Enable global interrupts.
    sei();

    while (1)
    {
        // Nothing required here. Count is updated in ISR.
    }

    return 0;
}
