#ifndef PEOPLE_COUNTER_H
#define PEOPLE_COUNTER_H

// Load people count from EEPROM
void load_people_count_from_EEPROM();

// Save people count to EEPROM
void save_people_count_to_EEPROM(unsigned int peopleCount);

// External reference to people count
extern unsigned int people_count;

#endif /* PEOPLE_COUNTER_H */