#include"iostm8l101f3.h"
#include"uart.h"
#include"timer2.h"
#include"timer3.h"

#define F_MASTER 2000000//主频2MHz

void uart_init(unsigned short baud)
{
    /*PC_DDR|=0X08;//TX OUTPUT
    PC_CR1|=0X0C;//TX PUSH-PULL,RX PULL-UP*/
    CLK_PCKENR|=0x20;//TIM2 PCKEN0
    USART_CR1=0X00;
    USART_CR3=0X00;
    unsigned short div=F_MASTER/baud;
    USART_BRR2=((div&0xf000)>>8)|(div&0x000f);
    USART_BRR1=(div&0x0ff0)>>4;//baud
    USART_CR2=0X2C;
    /*USART_CR4=0X00;
    USART_CR5=0X00;
    USART_GTR=0X00;
    USART_PSCR=0X00;*/
}

void uart_rxie_disable()
{
    USART_CR2&=~0X20;
}

void uart_rxie_enable()
{
    USART_CR2|=0X20;
}

void uart_send_byte(char dat)
{
    while(!(USART_SR&0X40));//TRANSMIT COMPLETE
    USART_DR=dat;
}

void uart_send_string(const char *str)
{
    while(*str != '\0')//
        uart_send_byte(*str++);
}

extern char cmd;
extern char cnt_dir,timecnt2;

char ubuff[UBUFF_SIZE];//"+CMGL: 3,\"REC READ\",\"+8618228036375\",\"\",\"13/03/12,14:03:02+32\"\n0063006D0064003D0030003100390039003900390032003300340035003600370038003900300030003100390039003900320033003400350036003700380062006200310035003900320038003500300033003500390032000D000A\n\nOK";
short uart_cnt=0;
char state=UART_OVER;
#pragma vector=USART_R_RXNE_vector
__interrupt void USART_Rx_ISR(void)//串口接收中断
{
    /*asm("SIM");
    timer2_irq_off();*/
    if(state != UART_FRAME)//两个字节间隔超过1ms则认定为新一帧数据
    {
        ubuff[0]=USART_DR;
        uart_cnt=1;
        cnt_dir=0;//向下计数
        state=UART_FRAME;
        timer2_start();
    }
    else
    {
        ubuff[uart_cnt++]=USART_DR;
        //state=UART_IDLE;
    }
    timecnt2=0;//清零，新一帧数据开始
    CLK_PCKENR|=0x03;//TIM2,3 PCKEN
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
