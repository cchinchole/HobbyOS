#include <arch/i686/panic.h>
#include <drivers/vga.h>

void panic_vanilla(char *reason, char *file, uint32_t line)
{
    /*
	if (!usermode_started) {
		asm ("cli");
		tty_attr(RED_BG | BLACK);
		tty_puts("\nKernel Panic: ");
		tty_puts(reason);
		tty_puts("\nError encountered at ");
		tty_puts(file);
		tty_puts(", line ");
		tty_putdec(line);
		tty_putc('\n');
		asm ("hlt");
		for (;;);
	} else {
		tty_attr(RED_BG | BLACK);
		tty_puts("\nFatal error: ");
		tty_puts(reason);
		KillCurrent();
	}
    */
        clear_screen();
		asm ("cli");
        kprintf("Kernel panic: %s\n", reason);
        kprintf("Occurred in \" %s \" at line: %d\n", file, line);
		asm ("hlt");
		for (;;);
}

void assert_panic(char *file, uint32_t line, char *fault)
{
	asm ("cli");
    /*
	tty_attr(RED_BG | BLACK);
	tty_puts("\nKernel panic: assertion failed at ");
	tty_puts(file);
	tty_puts(", line ");
	tty_putdec(line);
	tty_puts(": ");
	tty_puts(fault);
	tty_putc('\n');
	asm ("hlt");
    */
	for (;;);
}
