#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= 0x0F; // Set PB0, PB1, PB2, PB3 as output 
    
    while(1) {
        PORTB |= 0x0F;  // All 4 LEDs ON 
        _delay_ms(1000); // 1 second delay
        
        PORTB &= ~0x0F; // All 4 LEDs OFF
        _delay_ms(1000);
    }
}