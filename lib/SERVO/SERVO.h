#ifndef SERVO_H
#define SERVO_H
#include <stdint.h> // Ensure this header is included for fixed-width integer types
void timer0_ctc_init();
void delay_ms_timer0(uint8_t delay_ms);
void servo_rotate(uint8_t angle);
void servo_init();

#endif // SERVO_H