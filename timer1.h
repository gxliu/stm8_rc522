#ifndef _TIMER1_H_
#define _TIMER1_H_

void timer1_init();
void timer1_start();
void timer1_stop();
void timer1_irq_on();
void timer1_irq_off();
void timer1_wait_s(unsigned short t);

#endif
