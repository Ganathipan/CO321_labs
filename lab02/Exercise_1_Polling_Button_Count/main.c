/*
 * CO321 Embedded Systems - Lab 2: Interrupts
 * Exercise 1: Polling button counter
 *
 * Task:
 * PD7 pin is connected to a push button.
 * Use 6 LEDs connected to PORTB lower 6 bits PB0-PB5.
 * Display the number of times the button is pressed as a binary number.
 * Implement WITHOUT external interrupts, using polling.
 *
 * Wiring assumption:
 * - Button is connected between PD7 and GND.
 * - Internal pull-up is enabled on PD7.
 * - Not pressed = HIGH, pressed = LOW.
 * - LEDs are connected to PB0-PB5 through resistors.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BUTTON_PIN      PD7
#define LED_MASK        0x3F        // PB0-PB5 = lower 6 bits
#define DEBOUNCE_MS     150

static void display_count(uint8_t count)
{
    // Keep upper bits of PORTB unchanged and update only PB0-PB5.
    PORTB = (PORTB & ~LED_MASK) | (count & LED_MASK);
}

int main(void)
{
    uint8_t count = 0;

    // Configure PB0-PB5 as outputs for six LEDs.
    DDRB |= LED_MASK;

    // Start display at 0.
    display_count(count);

    // Configure PD7 as input.
    DDRD &= ~(1 << BUTTON_PIN);

    // Enable internal pull-up resistor on PD7.
    PORTD |= (1 << BUTTON_PIN);

    while (1)
    {
        // Because pull-up is used, pressed means the pin reads LOW.
        if (!(PIND & (1 << BUTTON_PIN)))
        {
            // Simple debounce: wait and check again.
            _delay_ms(DEBOUNCE_MS);

            if (!(PIND & (1 << BUTTON_PIN)))
            {
                // Count one valid press.
                count = (count + 1) & LED_MASK;  // Keep count in 0-63 range.
                display_count(count);

                // Wait until button is released to avoid counting the same press repeatedly.
                while (!(PIND & (1 << BUTTON_PIN)))
                {
                    // stay here while button is held down
                }

                // Debounce the release also.
                _delay_ms(DEBOUNCE_MS);
            }
        }
    }

    return 0;
}
