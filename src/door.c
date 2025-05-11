#include <avr/io.h>
#include <util/delay.h>
#include "../lib/BIT_MATH.h"
#include "../lib/SERVO/SERVO.h"
#include "door.h"

void open_close_door()
{
    // Open the door
    servo_rotate(1.5);
    SET_BIT(PORTC, 1); // Door open LED on
    CLR_BIT(PORTC, 0); // Door close LED off
    _delay_ms(2000);   // Wait for 2 seconds

    // Close the door
    servo_rotate(2);
    SET_BIT(PORTC, 0); // Door close LED on
    CLR_BIT(PORTC, 1); // Door open LED off
}