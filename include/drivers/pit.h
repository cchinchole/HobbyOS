#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void install_timer(uint32_t freq);
void uninstall_timer();

#endif