#include"iostm8s103f2.h"
#include"timer3.h"
#include"main.h"
#include"uart.h"
/*1S级延时*/

void timer3_init()
{
	//CLK_PCKENR|=0x02;//TIM3 PCKEN0:STM8L
    TIM3_IER=0X01;//UIE
    TIM3_EGR=0X01;
    TIM3_PSCR=7;//2^7=128分频
    TIM3_ARRH=0x3d;//定时1s
    TIM3_ARRL=0x09;//定时1s
    TIM3_CR1=0X95;//down-counter
}

void timer3_start()
{
    TIM3_CR1=0x00;
    TIM3_CNTRH=TIM3_ARRH;
    TIM3_CNTRL=TIM3_ARRL;
    TIM3_CR1=0x95;
}

void timer3_stop()
{
    TIM3_CR1=0x00;
}

void timer3_irq_on()
{
    TIM3_IER=0x01;
}

void timer3_irq_off()
{
    TIM3_IER=0X00;
    TIM3_SR1&=~0X01;//clear UIF bit
}

char timeout3=1;//向下计数
#pragma vector=TIM3_OVR_UIF_vector//定时1ms溢出
__interrupt void timer3_overflow_ISR()
{
    TIM3_SR1&=~0X01;//clear UIF bit
    if(--timeout3)
        return;
    timer3_irq_off();
}

void timer3_wait_s(char t)
{
    timer3_irq_on();
    timer3_start();
    timeout3=t;
    while(timeout3);
}

void set_timer3_wait_s(char t)
{
    timeout3=t;
    timer3_irq_on();
    timer3_start();
}
