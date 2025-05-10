#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "../lib/LCD/LCD.h"
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include "../lib/BIT_MATH.h"
#include "../lib/KEYPAD/KEYPAD.h"
#include "../lib/EEPROM/EEPROM.h"
#include <../lib/SERVO/SERVO.h>

// Function Prototypes
void initializeEEPROM(unsigned char *pass);
void resetPass();
void init_project();
void print_welcome();
int enter_password();

char correct_pass[5] = "1234"; // Default password
unsigned int people_count = 0; // Initialize people count

int main(void)
{
    // Setting Up The Project
    init_project(); // Initialize servo

    char entered_pass[5] = {0}; // Initialize to zeros
    uint8_t pass_index = 0;

    print_welcome(); // Print welcome message
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
                    people_count++;
            }
            else if (k == '+')
            {
                // Reset the password
                resetPass();
            }
            else if (k == '-')
            {
                // Get out
            }
        }

        // Enter the password

        // Correct Password Screen

        // Open the door Screen and rotate the servo

        // Wrong Password Screen

        // Reset the password

        // Print the welcome message
        print_welcome(); // Print welcome message
        _delay_ms(1000); // Delay for 1 second
    }
}

void print_welcome()
{
    // Print welcome message on LCD
    LCD_gotoXY(0, 1);
    LCD_SendString("Welcome to SDL");
    LCD_gotoXY(0, 0);
    // Print people count on LCD
    LCD_SendString("People Count:");
    LCD_SendNumber(people_count);
}

void init_project()
{
    // Initialize the project
    LCD_Init();                     // Initialize LCD
    keypad_init();                  // Initialize keypad
    servo_init();                   // Initialize servo
    initializeEEPROM(correct_pass); // Initialize EEPROM with default password
}

void resetPass()
{
    // LCD Print RESET MODE
    // print each digit
    // after = new pass is set
    // LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
    LCD_SendString("Reset Mode");
    LCD_gotoXY(0, 1);
    char curr[5];
    int x = 0;
    char zz = keypad_getkey();
    while (zz != '=')
    {
        if (x <= 4)
        {
            if (zz != 0)
            {
                curr[x] = zz;
                LCD_SendData(zz);
                x++;
                _delay_ms(20);
            }
            else if (x > 3)
            {
                for (int i = 0; i < 4; i++)
                {
                    // Write the new password to EEPROM
                    EEPROM_write(i, curr[i]);
                    // Update the correct_pass array
                    correct_pass[i] = curr[i];
                }
                LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                break;
            }
            zz = keypad_getkey();
        }
    }
}

void initializeEEPROM(unsigned char *pass)
{
    // Initialize EEPROM with default password
    // Check if EEPROM is empty (or contains a specific value)
    // If empty, write the default password to EEPROM
    if (EEPROM_read(0x0) == 0xFF) // Assuming 0xFF indicates empty
    {
        for (int i = 0; i < 4; i++)
        {
            EEPROM_write(i, pass[i]);
        }
    }
    else
    {
        // Read the password from EEPROM into the correct_pass array
        for (int i = 0; i < 4; i++)
        {
            correct_pass[i] = EEPROM_read(i);
        }
    }
}

int enter_password()
{
    // LCD Print Enter Password
    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
    LCD_SendString("Enter Password:");
    LCD_gotoXY(0, 1);
    char entered_pass[5] = {0}; // Initialize to zeros
    uint8_t pass_index = 0;
    char k;
    while (1)
    {
        k = keypad_getkey();
        if (k != 0)
        {
            _delay_ms(50); // Debounce delay

            // Only accept digits (ignore '=', 'C', etc.)
            if (k >= '0' && k <= '9')
            {
                if (pass_index < 4)
                {
                    entered_pass[pass_index] = k;
                    LCD_SendData('*'); // Show * instead of the actual digit
                    pass_index++;
                }
            }
            // If 'C' is pressed, clear input
            else if (k == 'C')
            {
                LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                pass_index = 0;
                memset(entered_pass, 0, sizeof(entered_pass));
            }
            // If '=' is pressed, check password
            else if (k == '=' && pass_index == 4)
            {
                entered_pass[4] = '\0'; // Null-terminate the string

                if (strcmp(entered_pass, correct_pass) == 0)
                {
                    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                    LCD_SendString("Correct!");
                    _delay_ms(1500); // Show message for 2 seconds
                    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                    return 1; // Password is correct
                }
                else
                {
                    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                    LCD_SendString("Wrong!");
                    _delay_ms(1500); // Show message for 2 seconds
                    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
                    return 0; // Password is wrong
                }
            }
        }
    }
}