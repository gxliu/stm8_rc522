#include"iostm8s103f3.h"
#include"timer2.h"
#include"uart.h"

void timer2_init()
{
    TIM2_IER=0X01;//UIE
    //TIM2_EGR=0X01;
    TIM2_PSCR=20;//20分频:2MHz/20=100kHz~10us
    /*TIM2_CNTRH=0;
    TIM2_CNTRL=0;*/
    TIM2_ARRH=100>>8;
    TIM2_ARRL=100&0xff;//定时100*10us=1ms
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

static unsigned short timeout2=1;
unsigned short timecnt2=0;
char cnt_dir=1;//1:up counter
#pragma vector=TIM2_OVR_UIF_vector//定时1ms溢出
__interrupt void timer2_overflow()
{
    TIM2_SR1&=~0X01;//clear UIF bit
	if(cnt_dir)
	{
		if(timecnt2++ == 2)
			uart_state=UART_OVER;
		else
			return;
	}
    else if(--timeout2)
        return;
    timer2_stop();
}

void timer2_wait_ms(unsigned short t)
{
    timeout2=t;
	cnt_dir=0;//向下计数
    timer2_start();
    while(timeout2);
}
