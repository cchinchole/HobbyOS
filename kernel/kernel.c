#include <libc/stdio.h>
#include <kernel/interrupts/isr.h>
#include <kernel/interrupts/gdt.h>
#include <drivers/pit.h>
#include <drivers/keyboard.h>
#include <kernel/interrupts/panic.h>
#include <drivers/vga.h>

void kernel_main()
{
    init_vga();
    gdt_install();
    isr_install();
    install_keyboard();
    kprintf("> ");
    for(;;){}
}
