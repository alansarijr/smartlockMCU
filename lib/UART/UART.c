#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "uart.h"

// F_CPU should be defined in the Makefile or project settings
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void UART_Init(uint32_t baud)
{
    // Calculate UBRR value for the given baud rate
    uint16_t ubrr = (F_CPU / 16 / baud) - 1;

    // Set baud rate
    UBRRL = (uint8_t)(ubrr & 0xFF);
    UBRRH = (uint8_t)(ubrr >> 8);

    // Enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void UART_SendChar(char data)
{
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)))
        ;

    // Put data into buffer, sends the data
    UDR = data;
}

void UART_SendString(const char *str)
{
    // Send each character in the string
    while (*str)
    {
        UART_SendChar(*str++);
    }
}

void UART_SendNumber(uint16_t num)
{
    char buffer[7]; // Max 65535 + null terminator

    // Convert the number to a string
    itoa(num, buffer, 10);

    // Send the string
    UART_SendString(buffer);
}

char UART_ReceiveChar(void)
{
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC)))
        ;

    // Get and return received data from buffer
    return UDR;
}

uint8_t UART_DataAvailable(void)
{
    // Return non-zero if data is available
    return (UCSRA & (1 << RXC));
}