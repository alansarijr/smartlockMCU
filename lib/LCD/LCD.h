#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT      PORTA
#define LCD_DDR       DDRA

// Define the pin numbers on the port for RS, E, and data lines
#define LCD_RS_PIN    PA1 
#define LCD_E_PIN     PA2 

#define LCD_D4_PIN    PA3 
#define LCD_D5_PIN    PA4 
#define LCD_D6_PIN    PA5 
#define LCD_D7_PIN    PA6 

// Commands for the LCD
#define LCD_CMD_CLEAR_DISPLAY           0x01 
#define LCD_CMD_RETURN_HOME             0x02 
#define LCD_CMD_ENTRY_MODE_INC_NOSHIFT  0x06 
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF   0x0C 
#define LCD_CMD_FUNCTION_SET_4BIT_2LINE 0x28 
#define LCD_CMD_SET_DDRAM_ADDR          0x80 


void LCD_Init(void);
void LCD_SendCMD(unsigned char cmd);
void LCD_SendData(unsigned char data);

/* Implement these functions */
void LCD_SendString(const char *str);
void LCD_gotoXY(unsigned char x, unsigned char y);
void LCD_SendNumber(int number);

#endif /* LCD_H_ */
