#include"iostm8s103f3.h"
#include"uart.h"
#include"timer2.h"
#include"timer3.h"

#define F_MASTER 2000000//主频2MHz

void uart_init(unsigned short baud)
{
    PD_DDR|=0X20;//TX OUTPUT
    PD_CR1|=0X60;//TX PUSH-PULL,RX PULL-UP
    UART1_CR1=0X00;
    UART1_CR3=0X00;
    unsigned short div=F_MASTER/baud;
    UART1_BRR2=((div&0xf000)>>8)|(div&0x000f);
    UART1_BRR1=(div&0x0ff0)>>4;//baud
    UART1_CR2=0X2E;
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

void uart_send_cnt(unsigned char *dat,char cnt)
{
	cnt++;
	for(char i=0;i<cnt;i++)
	{
		uart_send_byte(dat[i]);
	}
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
	while(!(UART1_SR&MASK_UART1_SR_RXNE));//0x20
	return UART1_DR;
}


extern char cmd;
unsigned char ubuff[UBUFF_SIZE];
char cnt=0;
char uart_state=UART_IDLE;
#pragma vector=UART1_R_RXNE_vector
__interrupt void UART1_Rx_ISR(void)//串口接收中断
{
    if(timecnt2 > 2)//两个字节间隔超过1ms则认定为新一帧数据
    {
        ubuff[0]=UART1_DR;
        cnt=1;
		cnt_dir=1;//向上计数
        timer2_start();
        uart_state=UART_FRAME;
    }
    else
    {
        ubuff[cnt++]=UART1_DR;
        //state=UART_IDLE;
    }
    timecnt2=0;//清零，新一帧数据开始
}
