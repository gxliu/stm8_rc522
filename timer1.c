#include"iostm8s103f2.h"
#include"timer1.h"
//#include"uart.h"

void timer1_init()
{
    TIM1_IER=0X01;//UIE
    //TIM1_EGR=0X01;
    TIM1_PSCRH=0;
    TIM1_PSCRL=200;//200分频
    TIM1_ARRH=10000>>8;
    TIM1_ARRL=10000&0xff;//定时1S
    //TIM1_CR1=0X95;//down-counter enable
}

void timer1_start()
{
    TIM1_CNTRH=TIM1_ARRH;
    TIM1_CNTRL=TIM1_ARRL;
    TIM1_CR1=0X95;//down-counter enable
}

void timer1_stop()
{
    TIM1_CR1=0x00;
}

void timer1_irq_on()
{
    TIM1_IER=0x01;
}

void timer1_irq_off()
{
    TIM1_IER=0X00;
    TIM1_SR1&=~0X01;//clear UIF bit
}

/*#pragma vector=TIM1_CAPCOM_CC1IF_vector//
__interrupt void timer1_capture_ch1()
{

}*/

char timeout1=1;
#pragma vector=TIM1_OVR_UIF_vector//定时1S溢出
__interrupt void timer1_overflow()
{
    TIM1_SR1&=~0X01;//clear UIF bit
    if(--timeout1)
        return;
    timer1_stop();
}

void timer1_wait_s(char t)
{
    //timer1_irq_on();
    timer1_start();
    timeout1=t;
    while(timeout1);
}
