#ifndef _UART_H_
#define _UART_H_

#define RXD	        0X04//PC2
#define TXD	        0X08//PC3

#define UART_IDLE	0
#define UART_START	1
#define	UART_FRAME	2
#define	UART_OVER	3

#define UBUFF_SIZE  300

void uart_init(unsigned short baud);
void uart_send_byte(char dat);
void uart_send_string(const char *str);
void uart_wait_receive(char opt);
void uart_rxie_enable();
void uart_rxie_disable();

#define uart_putc(dat)  uart_send_byte(dat)
#define uart_puts(str)  uart_send_string(str)

#endif