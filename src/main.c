#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "../lib/LCD/LCD.h"
#include "../lib/BIT_MATH.h"
#include "../lib/KEYPAD/KEYPAD.h"
#include "../lib/EEPROM/EEPROM.h"
#include "../lib/SERVO/SERVO.h"
#include "password.h"
#include "door.h"
#include "people_counter.h"
#include "user_interface.h"
#include "uart.h"

#define F_CPU 16000000UL
#define UART_BAUD_RATE 9600

int main(void)
{
    // Setting Up The Project
    init_project();

    // Initialize UART
    UART_Init(UART_BAUD_RATE);

    // Welcome message on UART
    UART_SendString("============================\r\n");
    UART_SendString("SDL Access Control System\r\n");
    UART_SendString("UART Monitoring Activated\r\n");
    UART_SendString("============================\r\n");

    // Load people count from EEPROM
    load_people_count_from_EEPROM();

    // Initial servo position
    servo_rotate(2);

    // Set PORTC as output for LCD and LEDs
    DDRC = 0xFF;
    PORTC = 0x01; // Initialize PORTC to 0

    // Display welcome message
    print_welcome();

    while (1)
    {
        // Password Screen
        char k = keypad_getkey();
        if (k != 0)
        {
            if (k == '*')
            {
                // Enter the password
                int ss = enter_password();
                if (ss == 1)
                {
                    // Increment people count, save to EEPROM, and send via UART
                    update_people_count(1);
                    UART_SendString("Access granted: Person entered\r\n");
                    open_close_door();
                }
                else
                {
                    // Wrong password indication
                    UART_SendString("Access Denied: Wrong Password\r\n");
                    SET_BIT(PORTC, 5);
                    _delay_ms(2000);
                    CLR_BIT(PORTC, 5);
                }
            }
            else if (k == '+')
            {
                // Reset the password
                LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                resetPass();
            }
            else if (k == '-')
            {
                // Decrement people count, save to EEPROM, and send via UART
                update_people_count(-1);
            }
        }
        print_welcome(); // Refresh welcome message
    }
}