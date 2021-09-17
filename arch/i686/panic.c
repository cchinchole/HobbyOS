#include <arch/i686/panic.h>
#include <drivers/vga.h>

void panic_vanilla(char *reason, char *file, uint32_t line)
{
		asm ("cli");
        clear_screen();
		set_attribute(BLACK_BG | RED);
        kprintf("Kernel panic: %s\n", reason);
        kprintf("Occurred in \" %s \" at line: %d\n", file, line);
		asm ("hlt");
		for (;;);
}

void assert_panic(char *file, uint32_t line, char *fault)
{
	asm ("cli");
    clear_screen();
	set_attribute(BLACK_BG | RED);
    kprintf("Kernel panic: assertion failed at %s, line %d: %s\n", file, line, fault);
	asm ("hlt");
    
	for (;;);
}
