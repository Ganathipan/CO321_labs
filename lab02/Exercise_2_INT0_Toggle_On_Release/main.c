/*
 * CO321 Embedded Systems - Lab 2: Interrupts
 * Exercise 2: Toggle PB0 LED whenever push button is released using external interrupt
 *
 * Task:
 * Write a program that toggles pin PB0 whenever the push button is released.
 * Use external interrupt INT0.
 *
 * Wiring assumption:
 * - Button is connected between PD2/INT0 and GND.
 * - Internal pull-up is enabled on PD2.
 * - Pressed = LOW, released = HIGH.
 * - Release event = rising edge.
 * - LED is connected to PB0 through a resistor.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTON_INT0_PIN PD2
#define LED_PIN         PB0
#define DEBOUNCE_MS     25

ISR(INT0_vect)
{
    // Simple debounce inside ISR for lab demonstration.
    // In production code, timer-based debounce is preferred.
    _delay_ms(DEBOUNCE_MS);

    // Confirm that the button is now released.
    // With pull-up wiring, released means PD2 is HIGH.
    if (PIND & (1 << BUTTON_INT0_PIN))
    {
        PORTB ^= (1 << LED_PIN);    // Toggle PB0 LED.
    }
}

int main(void)
{
    // Configure PB0 as output.
    DDRB |= (1 << LED_PIN);

    // Initially turn LED off.
    PORTB &= ~(1 << LED_PIN);

    // Configure PD2/INT0 as input.
    DDRD &= ~(1 << BUTTON_INT0_PIN);

    // Enable internal pull-up on PD2.
    PORTD |= (1 << BUTTON_INT0_PIN);

    // Configure INT0 to trigger on rising edge.
    // Rising edge = LOW to HIGH = button release with pull-up wiring.
    EICRA |= (1 << ISC01) | (1 << ISC00);

    // Enable external interrupt INT0.
    EIMSK |= (1 << INT0);

    // Enable global interrupts.
    sei();

    while (1)
    {
        // Main program can do other work here.
        // The LED toggling happens automatically inside ISR(INT0_vect).
    }

    return 0;
}
