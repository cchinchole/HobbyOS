#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>
#include "types.h"

#define panic(a) (panic_vanilla(a, __FILE__, __LINE__))
#define assert(a) ((a) ? (void)0 : assert_panic(__FILE__, __LINE__, #a))

#endif