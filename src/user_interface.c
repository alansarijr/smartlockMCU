#include "../lib/LCD/LCD.h"
#include "../lib/KEYPAD/KEYPAD.h"
#include "../lib/SERVO/SERVO.h"
#include "password.h"
#include "people_counter.h"
#include "user_interface.h"
#include "../lib/UART/UART.h"

void init_project()
{
    // Initialize the project components
    LCD_Init();                     // Initialize LCD
    keypad_init();                  // Initialize keypad
    servo_init();                   // Initialize servo
    initializeEEPROM(correct_pass); // Initialize EEPROM with default password
}

void print_welcome()
{
    // Print welcome message on LCD
    LCD_SendString("Welcome to SDL");

    // Print people count on LCD
    LCD_gotoXY(0, 1);
    LCD_SendString("People Count:");
    LCD_SendNumber(people_count);
}