#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#include  <libc/stddef.h>

void install_inf_timer(bool doShowTick, uint32_t freq);
void install_tick_timer(uint32_t freq, int mTick);
int give_tick();
int get_elapsed_tick();
void reset_timer();
void uninstall_timer();

#endif