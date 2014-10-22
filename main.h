#ifndef _MAIN_H_
#define _MAIN_H_

#define LED_R         0X10//PB4
#define LED_B         0X08//PB3
#define LED_G         0X04//PB2
#define BEEP          0X02//PB1

#define CMD_SLEEP		0x00
#define CMD_SYSY_RST	0x01
#define CMD_SYS_INFO    0x02
#define CMD_REPORT  	0x11
#define CMD_BEEP      	0x12
#define CMD_LED_CTRL    0x13

#define mcu_sleep()     asm("WFI")//WFI:Enter Wait Mode to save power while peripherals keep running

#define LED_ON(LED)     PB_ODR&=~(LED)
#define LED_OFF(LED)    PB_ODR|=(LED)
#define BEEP_ON()       PB_ODR|=BEEP
#define BEEP_OFF()      PB_ODR&=~BEEP

void sys_init(void);
void sys_reset(void);

#endif