#ifndef TIMER_H
#define TIMER_H

typedef void (*timer_callback_t)(void);

void configure_timer_0(timer_callback_t cb_func);

#endif