#ifndef IO_H
#define IO_H
#include <stdint.h>

uint8_t pbin(uint16_t port_num);
void pbout(uint16_t port_num, uint8_t data);

#endif