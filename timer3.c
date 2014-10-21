#include"iostm8l101f2.h"
#include"timer3.h"
#include"uart.h"

void timer3_init()
{
	TIM3_IER=0X01;//UIE
	TIM3_EGR=0X01;
	TIM3_PSCR=20;//20分频
	TIM3_ARRH=100>>8;
	TIM3_ARRL=100&0xff;//定时1ms
	TIM3_CR1=0X95;//
}

void timer3_start()
{
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

char timeout3=0;//向下计数
#pragma vector=TIM3_OVR_UIF_vector//定时1ms溢出
__interrupt void timer3_overflow()
{
	TIM3_SR1&=~0X01;//clear UIF bit
	if(--timeout3)
		return;
	timer3_irq_off();
}

void timer3_wait_ms(char t)
{
	timer3_irq_on();
	//timer3_start();//这里不调用就得在其他地方先调用
	timeout3=t;
	while(timeout3);
}
