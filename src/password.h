#ifndef PASSWORD_H
#define PASSWORD_H

// Initialize password in EEPROM
void initializeEEPROM(unsigned char *pass);

// Reset password interface
void resetPass();

// Password entry and validation
int enter_password();

// External reference to the correct password
extern char correct_pass[5];

#endif /* PASSWORD_H */