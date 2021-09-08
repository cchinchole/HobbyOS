#include  <libc/stdio.h>
#include  <arch/i686/isr.h>
#include  <arch/i686/gdt.h>
#include  <drivers/pit.h>
#include  <drivers/keyboard.h>
#include  <arch/i686/panic.h>
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
