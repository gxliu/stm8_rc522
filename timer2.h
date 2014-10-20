#ifndef _TIMER2_H_
#define _TIMER2_H_

void timer2_init();
void timer2_start();
void timer2_stop();
void timer2_irq_on();
void timer2_irq_off();
void timer2_wait_ticks(short t);

#endif
