#include"iostm8s103f2.h"
#include"timer2.h"
#include"uart.h"

void timer2_init()
{
    TIM2_IER=0X01;//UIE
    //TIM2_EGR=0X01;
    TIM2_PSCR=0;//不分频:2MHz/0.5us
    /*TIM2_CNTRH=0;
    TIM2_CNTRL=0;*/
    TIM2_ARRH=1000>>8;
    TIM2_ARRL=1000&0xff;//1280定时640us
    //TIM2_CR1=0X85;//
}

void timer2_start()
{
    TIM2_CNTRH=0;
    TIM2_CNTRL=0;
    TIM2_CR1=0x85;//upcounter,enable
    //TIM2_CR1|=0x01;//enable
}

void timer2_stop()
{
    TIM2_CR1=0x00;
}

void timer2_irq_on()
{
    TIM2_IER=0x01;
}

void timer2_irq_off()
{
    TIM2_IER=0X00;
    TIM2_SR1&=~0X01;//clear UIF bit
}

extern char state;
short timeout2=1;
#pragma vector=TIM2_OVR_UIF_vector//定时512us溢出
__interrupt void timer2_overflow()
{
    TIM2_SR1&=~0X01;//clear UIF bit
    if(--timeout2)
        return;
    timer2_stop();
}

void timer2_wait_ticks(short t)
{
    timeout2=t;
    timer2_start();
    while(timeout2);
}
