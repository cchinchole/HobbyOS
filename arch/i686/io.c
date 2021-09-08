#include  <arch/i686/io.h>
#include <stdint.h>
uint8_t pbin(uint16_t port_num)
{
    uint8_t ret;
    asm volatile("inb %%dx, %%al" : "=a" (ret) : "d" (port_num));
    return ret;

}

void pbout(uint16_t port_num, uint8_t data)
{
      asm("out %%al, %%dx" : : "a" (data), "d" (port_num));
}