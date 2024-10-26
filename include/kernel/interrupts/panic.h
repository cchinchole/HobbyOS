#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>
#include "../types.h"

void panic_vanilla(char *reason, char *file, uint32_t line);
void panic_assert(char *file, uint32_t line, char *fault);

#define panic(a) (panic_vanilla(a, __FILE__, __LINE__))
#define assert(a) ((a) ? (void)0 : assert_panic(__FILE__, __LINE__, #a))

#endif
