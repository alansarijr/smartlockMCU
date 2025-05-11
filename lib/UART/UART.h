#ifndef UART_H
#define UART_H

#include <stdint.h>

// Initialize UART with specified baud rate
void UART_Init(uint32_t baud);

// Send a single character over UART
void UART_SendChar(char data);

// Send a string over UART
void UART_SendString(const char* str);

// Send an integer value over UART
void UART_SendNumber(uint16_t num);

// Receive a character from UART (blocking)
char UART_ReceiveChar(void);

// Check if data is available to read
uint8_t UART_DataAvailable(void);

#endif /* UART_H */