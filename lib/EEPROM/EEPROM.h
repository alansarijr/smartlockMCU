#ifndef EEPROM_H
#define EEPROM_H

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
void initializeEEPROM(unsigned char *pass);
unsigned char EEPROM_read(unsigned int uiAddress);
#endif // EEPROM_H