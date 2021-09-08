#ifndef KEYBOARD_H
#define KEYBOARD_H
#include  <arch/i686/isr.h>

char line[25];

void keyboard_callback(registers_t* r);

#endif