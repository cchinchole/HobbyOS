#include  <arch/i686/io.h>
#include  <arch/i686/isr.h>
#include  <libc/stdio.h>
#include  <libc/string.h>
#include  <drivers/pit.h>
#include  <drivers/vga.h>
#include  <stdint.h>

#define TIMER_CMD 0x43
#define TIMER_DATA 0x40

int tick = 0;
int maxTick = -1;
int startTick = 0;
bool showTick = false;
void uninstall_timer();

static void timer_callback(registers_t* r)
{
    tick++;
    if(showTick)
        kprintf("Timer tick %d\n", tick);

    if(maxTick != -1)
        if(tick >= maxTick)
        {
            uninstall_timer();
            tick = 0;
            maxTick = -1;
        }
    REG_PARAM(r);
}

int give_tick()
{
    return tick;
}

int get_elapsed_tick()
{
    return tick-startTick;
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

void install_inf_timer(bool doShowTick, uint32_t freq)
{
    showTick = doShowTick;
    maxTick = -1;
    startTick = tick;
    install_timer(freq);
}

void install_tick_timer(uint32_t freq, int mTick)
{
    maxTick = mTick;
    startTick = tick;
    kprintf("Max tick set to %d", maxTick);
    install_timer(freq);
}

void reset_timer()
{
    tick = 0;
    startTick = 0;
    maxTick = -1;
}

void uninstall_timer()
{
    kprintf("Uninstalling Timer.\n");
    register_interrupt_handler(IRQ0, 0);
}
