#ifndef _TIMER3_H_
#define _TIMER3_H_

void timer3_init();
void timer3_start();
void timer3_stop();
void timer3_irq_on();
void timer3_irq_off();
void timer3_wait_s(char t);
void set_timer3_wait_s(char t);

#endif
