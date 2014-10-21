#ifndef _MAIN_H_
#define _MAIN_H_

#define STDBY         0X08//PA3
#define CHARGE        0X04//PA2
#define STATUS        0X80//PB7
#define NETLIGHT      0X40//PB6
#define HALL          0X20//PB5
#define LED_R         0X10//PB4
#define LED_B         0X08//PB3
#define LED_G         0X04//PB2
#define BEEP          0X02//PB1
#define LOW_BAT       0X01//PB0
#define PWRKEY        0X01//PD0

#define CMD_SLEEP		0x00
#define CMD_REGISTER	0x01
#define CMD_SYS_INFO    0x02
#define CMD_REPORT  	0x11
#define CMD_BEEP      	0x12
#define CMD_LED_CTRL    0x13

#define MSG_SIZE        10//最多存储?条离线记录

#define MSG_TYPE_NORMAL     0x33
#define MSG_TYPE_LOW_BAT    0x88
#define MSG_TYPE_EXTEND     0x99

#include"eeprom.h"
#if USE_EEPROM == INTERNAL
#define DEV_INFO        (char*)0x9D20
#define REG_FLAG        (char*)0x9D46
#define INDEX           (char*)0x9D47
#define RECORD          (char*)0x9D48
#else
#define REG_FLAG        0x00
#define DEV_INFO        0x01
#define INDEX           0x27
#define RECORD          0x28
#endif

//#define MY_TEST
#ifdef MY_TEST
#define SERVER_IP       "www.baidu.com"
#define SERVER_PORT     "80"
#else
#define SERVER_IP       "125.71.214.8"
#define SERVER_PORT     "12345"
#endif

#define mcu_sleep()     asm("WFI")//WFI:Enter Wait Mode to save power while peripherals keep running
/*
const char sca_str[]="8613800280500";
const char da_str[] ="8615882401343";
const char cmgl_str3[]="+CMGL: 3,\"REC READ\",\"+8618228036375\",\"\",\"13/03/12,14:03:02+32\"\n0063006D0064003D0030003100350031003000310032003300340035003600370038003900300032\n\nOK";
        //cmd=0151012345678900151123456789015928503592
const char creg_str[]="+CREG: 2,1,\"821A\",\"EC5F\"\n\nOK";
*/

#define LED_ON(LED)     PB_ODR&=~(LED)
#define LED_OFF(LED)    PB_ODR|=(LED)
#define BEEP_ON()       PB_ODR|=BEEP
#define BEEP_OFF()      PB_ODR&=~BEEP

void sys_init(void);
void save_record();;

#endif