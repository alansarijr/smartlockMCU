#include "./LCD.h"
#include "../BIT_MATH.h"
#define F_CPU 16000000UL
static void LCD_Pulse_E(void)
{
    SET_BIT(LCD_PORT, LCD_E_PIN);
    _delay_us(1);
    CLR_BIT(LCD_PORT, LCD_E_PIN);
    _delay_us(1);
}

static void LCD_SendNibble(unsigned char nibble)
{
    LCD_PORT &= ~((1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN));

    if (GET_BIT(nibble, 0))
        SET_BIT(LCD_PORT, LCD_D4_PIN);
    if (GET_BIT(nibble, 1))
        SET_BIT(LCD_PORT, LCD_D5_PIN);
    if (GET_BIT(nibble, 2))
        SET_BIT(LCD_PORT, LCD_D6_PIN);
    if (GET_BIT(nibble, 3))
        SET_BIT(LCD_PORT, LCD_D7_PIN);

    LCD_Pulse_E();
}

void LCD_SendCMD(unsigned char cmd)
{
    // Set RS low for command mode
    CLR_BIT(LCD_PORT, LCD_RS_PIN);

    // Send high nibble
    LCD_SendNibble((cmd >> 4) & 0x0F);

    // Send low nibble
    LCD_SendNibble(cmd & 0x0F);

    // Wait for command execution time
    if (cmd == LCD_CMD_CLEAR_DISPLAY || cmd == LCD_CMD_RETURN_HOME)
    {
        _delay_ms(2); // Needs > 1.53ms
    }
    else
    {
        _delay_us(50); // Most others need > 39us
    }
}

void LCD_SendData(unsigned char data)
{
    // Set RS high for data mode
    SET_BIT(LCD_PORT, LCD_RS_PIN);

    // Send high nibble
    LCD_SendNibble((data >> 4) & 0x0F);

    // Send low nibble
    LCD_SendNibble(data & 0x0F);

    // Wait for data write time (> 43us typically required)
    _delay_us(50);
}

void LCD_Init(void)
{
    // Configure LCD port pins (RS, E, D4-D7) as outputs
    LCD_DDR |= (1 << LCD_RS_PIN) | (1 << LCD_E_PIN) | (1 << LCD_D4_PIN) | (1 << LCD_D5_PIN) | (1 << LCD_D6_PIN) | (1 << LCD_D7_PIN);

    _delay_ms(40); // Wait > 30ms after power on

    CLR_BIT(LCD_PORT, LCD_RS_PIN);
    LCD_SendNibble(0x02);

    LCD_SendCMD(LCD_CMD_FUNCTION_SET_4BIT_2LINE);
    _delay_us(100);

    LCD_SendCMD(LCD_CMD_DISPLAY_ON_CURSOR_OFF);
    _delay_us(100);

    LCD_SendCMD(LCD_CMD_CLEAR_DISPLAY);
    _delay_ms(2);

    LCD_SendCMD(LCD_CMD_ENTRY_MODE_INC_NOSHIFT);
}

/* Implement these functions */
void LCD_SendString(const char *str)
{
    while (*str)
    {
        LCD_SendData(*str++);
    }
}
// void LCD_gotoXY(unsigned char x, unsigned char y) {

// }
// void LCD_SendNumber(int number) {

// }

void LCD_gotoXY(unsigned char x, unsigned char y)
{
    // DDRAM address mapping:
    // Line 0: 0x00 to 0x27
    // Line 1: 0x40 to 0x67
    unsigned char address;

    if (y == 0)
    {
        address = 0x00 + x; // First line
    }
    else
    {
        address = 0x40 + x; // Second line
    }

    // Ensure address is within valid range
    if (address < 0x80)
    { // Max DDRAM address is 0x7F
        LCD_SendCMD(LCD_CMD_SET_DDRAM_ADDR | address);
    }
}

void LCD_SendNumber(int number)
{
    char buffer[16]; // Enough to hold -32768 to 32767
    int i = 0;

    if (number == 0)
    {
        LCD_SendData('0');
        return;
    }

    if (number < 0)
    {
        LCD_SendData('-');
        number = -number;
    }

    // Build the string in reverse order
    while (number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }

    // Send digits in correct order
    while (i > 0)
    {
        LCD_SendData(buffer[--i]);
    }
}
