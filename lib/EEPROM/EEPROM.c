#include <avr/io.h>
#include <util/delay.h>
#include <EEPROM.h>
#include <avr/eeprom.h>

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    /* Wait for completion of previous write */
    while (EECR & (1 << EEWE))
        ;
    /* Set up address and data registers */
    EEAR = uiAddress;
    EEDR = ucData;
    /* Write logical one to EEMWE */
    EECR |= (1 << EEMWE);
    /* Start eeprom write by setting EEWE */
    EECR |= (1 << EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
    /* Wait for completion of previous write */
    while (EECR & (1 << EEWE))
        ;
    /* Set up address register */
    EEAR = uiAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1 << EERE);
    /* Return data from data register */
    return EEDR;
}
