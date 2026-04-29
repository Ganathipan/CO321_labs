#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 200

int main(void)
{
    // configure pins of PORTB for output
    DDRB = DDRB | (1 << 4);
    DDRB = DDRB | (1 << 3);
    DDRB = DDRB | (1 << 2);
    DDRB = DDRB | (1 << 1);
    DDRB = DDRB | (1 << 0);

    // initialize variables for controlling the LED sequence
    int lf = 0, rg = 4, cur = 0, pre = 0, dir = 1;
    
    while(1) {
        // initialize variables for controlling the LED sequence
        int lf = 0, rg = 4, cur = 0, pre = 0, dir = 1;
        
        // set all pins of PORTB low to turn all leds off
        PORTB = 0x00;

        while (lf <= cur && cur <= rg) {
            PORTB = PORTB | (1 << cur);
            _delay_ms(BLINK_DELAY_MS);

            if (cur != pre) {
                PORTB = PORTB & ~(1 << pre);
                _delay_ms(BLINK_DELAY_MS);
            }
            pre = cur;
            cur = cur + dir;
            
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
        PORTB = 0x00; // turn all leds off at the end of the sequence
    }
    return 0;
}