#ifndef _IWDG_H_
#define _IWDG_H_

#define KEY_ENABLE  0XCC
#define KEY_ACCESS  0X55
#define KEY_REFRESH 0XAA

void iwdg_enable(void);
void iwdg_disable(void);
void iwdg_refresh(void);

#endif