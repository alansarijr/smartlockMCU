#include <avr/io.h>
#include "../lib/EEPROM/EEPROM.h"
#include "people_counter.h"
#include "../lib/UART/UART.h"
#include "../lib/LCD/LCD.h"
#include "../lib/BIT_MATH.h"

// Global people count variable
unsigned int people_count = 0;

void save_people_count_to_EEPROM(unsigned int peopleCount)
{
    // Store peopleCount as two bytes (low and high) at EEPROM addresses 4 and 5
    EEPROM_write(4, (uint8_t)(peopleCount & 0xFF));        // Low byte
    EEPROM_write(5, (uint8_t)((peopleCount >> 8) & 0xFF)); // High byte
}

void load_people_count_from_EEPROM()
{
    // Read two bytes from EEPROM and reconstruct people_count
    uint8_t low = EEPROM_read(4);
    uint8_t high = EEPROM_read(5);

    if (low == 0xFF && high == 0xFF)
    {
        people_count = 0; // EEPROM is empty, default to 0
    }
    else
    {
        people_count = ((unsigned int)high << 8) | low;
    }

    // Send initial count via UART
    send_people_count_via_uart();
}

void update_people_count(int change)
{
    // Update the people count
    if (change > 0)
    {
        // Person entering
        people_count++;
    }
    else
    {
        // Person leaving
        if (people_count > 0)
        {
            people_count--;
            UART_SendString("Person exited\r\n");

            SET_BIT(PORTC, 2); // Door open LED on
            _delay_ms(2000);   // Wait for 2 seconds
            open_close_door();
            CLR_BIT(PORTC, 2); // Door open LED off
        }
        else
        {
            people_count = 0; // Prevent negative count
            save_people_count_to_EEPROM(people_count);
            LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
            LCD_SendString("No one inside");
            _delay_ms(2000);
            LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
            // Send status via UART
            UART_SendString("Warning: Attempted exit when count is 0\r\n");
            send_people_count_via_uart();

            _delay_ms(1000);
        }
    }

    // Save to EEPROM
    save_people_count_to_EEPROM(people_count);

    // Send via UART
    send_people_count_via_uart();
}

void send_people_count_via_uart()
{
    // Send a formatted message with the current people count
    UART_SendString("People Count: ");
    UART_SendNumber(people_count);
    UART_SendString("\r\n");
}