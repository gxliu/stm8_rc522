#include"iostm8s103f2.h"
#include"uart.h"
#include"timer2.h"
#include"timer3.h"

#define F_MASTER 2000000//主频2MHz

void uart_init(unsigned short baud)
{
	/*PC_DDR|=0X08;//TX OUTPUT
	PC_CR1|=0X0C;//TX PUSH-PULL,RX PULL-UP*/
	//CLK_PCKENR|=0x20;//TIM2 PCKEN0:STM8L
	UART1_CR1=0X00;
	UART1_CR3=0X00;
	unsigned short div=F_MASTER/baud;
	UART1_BRR2=((div&0xf000)>>8)|(div&0x000f);
	UART1_BRR1=(div&0x0ff0)>>4;//baud
	UART1_CR2=0X2C;
	/*UART1_CR4=0X00;
	UART1_CR5=0X00;
	UART1_GTR=0X00;
	UART1_PSCR=0X00;*/
}

void uart_rxie_disable()
{
	UART1_CR2&=~0X20;
}

void uart_rxie_enable()
{
	UART1_CR2|=0X20;
}

void uart_send_byte(char dat)
{
	while(!(UART1_SR&0X40));//TRANSMIT COMPLETE
	UART1_DR=dat;
}

void uart_send_string(const char *str)
{
	while(*str != '\0')//
		uart_send_byte(*str++);
}

void uart_puthex(char dat)
{
	char temp=dat>>4;
	if(0x00<temp && temp<0x09)
		uart_putc(temp+'0');
	else
		uart_putc(temp-0x0a+'A');
	temp=dat&0X0F;
	if(0x00<temp && temp<0x09)
		uart_putc(temp+'0');
	else
		uart_putc(temp-0x0a+'A');
}

char uart_getc(void)
{
	while(!(UART1_SR&0X08));//需要确认一下RXNE在那个bit
	return UART1_DR;
}


extern char cmd;
extern char cnt_dir,timecnt2;

char ubuff[UBUFF_SIZE];
short uart_cnt=0;
char state=UART_OVER;
#pragma vector=UART1_R_RXNE_vector
__interrupt void UART1_Rx_ISR(void)//串口接收中断
{
	/*asm("SIM");
	timer2_irq_off();*/
	if(state != UART_FRAME)//两个字节间隔超过1ms则认定为新一帧数据
	{
		ubuff[0]=UART1_DR;
		uart_cnt=1;
		cnt_dir=0;//向下计数
		state=UART_FRAME;
		timer2_start();
	}
	else
	{
		ubuff[uart_cnt++]=UART1_DR;
		//state=UART1_IDLE;
	}
	timecnt2=0;//清零，新一帧数据开始
	//CLK_PCKENR|=0x03;//TIM2,3 PCKEN:STM8L
	/*timer2_irq_on();
	asm("RIM");*/
}

extern char timeout3;
void uart_wait_receive(char opt)//opt=1, wait forever
{
	state=UART_IDLE;
	if(opt)
	{
		while(state != UART_OVER);
	}
	else
	{
		set_timer3_wait_s(10);
		while(state != UART_OVER && timeout3!=0);
	}
	ubuff[uart_cnt]='\0';
}
