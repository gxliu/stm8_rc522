#ifndef _EEPROM_H_
#define _EEPROM_H_

#define INTERNAL    0
#define EXTERNAL    1
#define USE_EEPROM  EXTERNAL

#if USE_EEPROM == EXTERNAL
    #include"at24c04.h"
#else
void eeprom_lock(void);
void eeprom_unlock(void);
void eeprom_write_byte(char *dst,char data);
char eeprom_read_byte(char *src);
void eeprom_write_block(char *dst,char *src,unsigned char cnt);
void eeprom_read_block(char *dst,char *src,unsigned char cnt);
void rop_enable();
void option_byte_write(char *addr, char dat);
#endif

#endif
