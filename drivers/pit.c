#include  <arch/i686/io.h>
#include  <arch/i686/isr.h>
#include  <libc/stdio.h>
#include  <libc/string.h>
#include  <drivers/pit.h>
#include  <drivers/vga.h>
#include <stdint.h>

#define TIMER_CMD 0x43
#define TIMER_DATA 0x40



int tick = 0;
static void timer_callback(registers_t* r)
{
    tick++;
    kprintf("Timer tick %d\n", tick);
    REG_PARAM(r);
}

void install_timer(uint32_t freq)
{
    kprintf("Installing Timer.\n");
    register_interrupt_handler(IRQ0, timer_callback);
    pbout(TIMER_CMD, 0x36);
    uint32_t div = 1193182 / freq;
    uint8_t low =  (uint8_t)((div) & 0xFF);
    uint8_t high = (uint8_t)((div>>8) & 0xFF);
    kprintf("Setting timer to %d\n", freq);
    pbout(TIMER_DATA, low);
    pbout(TIMER_DATA, high);
}

void uninstall_timer()
{
    kprintf("Uninstalling Timer.\n");
    register_interrupt_handler(IRQ0, 0);
}