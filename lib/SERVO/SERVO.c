#include "SERVO.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SERVO_PIN PB3

void timer0_ctc_init()
{
    TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00); // CTC mode, Prescaler 64
    OCR0 = 249;                                       // Compare every 1 ms (250 ticks * 4 µs = 1 ms)
}

void servo_init()
{
    // Set PB3 as output for servo control
    DDRB |= (1 << SERVO_PIN);
    // Initialize Timer0 in CTC mode
    timer0_ctc_init();
    // Enable Timer0 compare interrupt
    TIMSK |= (1 << OCIE0);
}

void delay_ms_timer0(uint8_t ms)
{
    for (uint8_t i = 0; i < ms; i++)
    {
        TCNT0 = 0;
        while ((TIFR & (1 << OCF0)) == 0)
            ;                // Wait for match
        TIFR |= (1 << OCF0); // Clear flag
    }
}

void servo_rotate(uint8_t angle)
{
    // Convert angle (0–180) to pulse width (1–2 ms)
    uint8_t pulse_width = 1 + ((angle * 1.0) / 180); // crude mapping, 1 to 2 ms
    // 1ms - > 0 angle and 2ms -> 180 angle, 1.5ms -> 90 angle

    for (int i = 0; i < 50; i++)
    {                                      // Send 50 pulses (~1 second)
        PORTB |= (1 << SERVO_PIN);         // HIGH
        delay_ms_timer0(pulse_width);      // Pulse width
        PORTB &= ~(1 << SERVO_PIN);        // LOW
        delay_ms_timer0(20 - pulse_width); // Remainder of 20ms
    }
}