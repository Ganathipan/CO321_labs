/*
 * CO321 Embedded Systems - Lab 1: LED Sequence
 * Exercise 1: LED sequence controller
 *
 * Task:
 * Use PORTB pins PB1-PB5 to run an LED sequence.
 * Light one LED at a time and move the active LED back and forth.
 *
 * Wiring assumption:
 * - LEDs are connected to PB1-PB5 through resistors.
 * - LEDs are active HIGH.
 */

#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 10001

int main(void)
{
    // Configure PB1-PB5 as outputs.
    DDRB = DDRB | (1 << 5);
    DDRB = DDRB | (1 << 4);
    DDRB = DDRB | (1 << 3);
    DDRB = DDRB | (1 << 2);
    DDRB = DDRB | (1 << 1);

    // Track the active LED window and movement direction.
    int lf = 1, rg = 5, cur = 1, pre = 1, dir = 1;

    // Start with every LED off.
    PORTB = 0x00;

    while (lf <= cur && cur <= rg) {
        // Turn on the current LED.
        PORTB = PORTB | (1 << cur);
        _delay_ms(BLINK_DELAY_MS);

        if (cur != pre) {
            // Turn off the previous LED so only one LED stays lit.
            PORTB = PORTB & ~(1 << pre);
            _delay_ms(BLINK_DELAY_MS);
        }

        pre = cur;
        cur = cur + dir;

        // Bounce the active LED window when either edge is reached.
        if (cur > rg) {
            dir = -1;
            rg = rg - 1;
            cur = rg;
        } else if (cur < lf) {
            dir = 1;
            lf = lf + 1;
            cur = lf;
        }
    }

    // Leave the LEDs off when the sequence ends.
    PORTB = 0x00;
    return 0;
}