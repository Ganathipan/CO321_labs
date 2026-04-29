#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= 0x0F; // Ensure PB0-PB3 are outputs 

    while(1) {
        // Forward: A -> B -> C -> D
        for(int i = 0; i < 4; i++) {
            PORTB = (1 << i); // Shift '1' to the i-th position 
            _delay_ms(200);
        }
        
        // Backward: C -> B (avoiding repeating D or A)
        for(int i = 2; i > 0; i--) {
            PORTB = (1 << i);
            _delay_ms(200);
        }
    }
}


