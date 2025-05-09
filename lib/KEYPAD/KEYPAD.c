#include "KEYPAD.h"
#include "../BIT_MATH.h"
#include <avr/io.h>
#include <util/delay.h>
char keypad[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'C', '0', '=', '+'}};

// Initialize keypad pins
void keypad_init()
{
    DDRD &= ~0x3C; // PD2–PD5 (columns) as input
    PORTD |= 0x3C; // Enable pull-up resistors on PD2–PD5

    DDRB |= (0xF0);  // PB4–PB7 (rows) as output
    PORTB |= (0xF0); // All row high
}

// Scan keypad and return pressed key, or 0 if none
char keypad_getkey()
{
    for (uint8_t row = 4; row < 8; row++)
    {
        CLR_BIT(PORTB, row); // Set row low

        for (uint8_t col = 2; col < 6; col++)
        {
            // Check if key is pressed
            if (GET_BIT(PIND, col) == 0)
            {
                _delay_ms(20); // Debounce delay
                while (GET_BIT(PIND, col) == 0)
                    ; // Wait for key release

                SET_BIT(PORTB, row);
                return keypad[row - 4][col - 2];
            }
        }
        SET_BIT(PORTB, row); // Set row high again
    }
    return 0; // No key pressed
}