#ifndef EEPROM_H
#define EEPROM_H

extern void eepromInit(void);
extern void eepromSetPassowrd(int temp_password);
extern unsigned long eepromGetPassword(void);

#endif
