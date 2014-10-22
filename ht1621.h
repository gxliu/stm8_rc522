#ifndef _HT1621_H_
#define _HT1621_H_

#include "iostm8s103f2.h"

//HT1621控制位（液晶模块接口定义，根据自已的需要更改）
#define HT1621_DDR		PA_DDR //
#define HT1621_ODR		PA_ODR //
#define HT1621_IDR		PA_IDR //
#define HT1621_CR1		PA_CR1 //
#define HT1621_CR2		PA_CR2 //
#define HT1621_DAT		PA_ODR_ODR0//0x01 //HT1621数据引脚
#define HT1621_CS		PA_ODR_ODR1 //HT1621使能引脚
#define HT1621_WR		PA_ODR_ODR2 //HT1621时钟引脚

#define HT_LCD_BIAS		0x52 //0b1000 0101 0010 1/3duty 4com
#define HT_SYS_OFF		0X00 //0b1000 0000 0000 关振系统荡器和LCD偏压发生器
#define HT_SYS_ON		0X02 //0b1000 0000 0010 打开系统振荡器

#define HT_LCD_OFF		0X04 //0b1000 0000 0100 关LCD偏压
#define HT_LCD_ON		0X06 //0b1000 0000 0110 打开LCD偏压
#define HT_USE_XTAL		0x28 //0b1000 0010 1000 外部接时钟
#define HT_USE_RC256	0X30 //0b1000 0011 0000 内部时钟
#define HT_TONE_ON		0X12 //0b1000 0001 0010 打开声音输出
#define HT_TONE_OFF		0X10 //0b1000 0001 0000 关闭声音输出
#define HT_WDT_OFF		0X0A //0b1000 0000 1010 禁止看门狗

void ht1621_init(void);
void ht1621_wr_dat(char dat,char cnt);
void ht1621_wr_cmd(char cmd);
void ht1621_wr_one(char addr,char dat);//
void ht1621_wr_cnt(char addr,char *p,char cnt);
void ht1621_clr_lcd(void);//clear lcd

#endif
