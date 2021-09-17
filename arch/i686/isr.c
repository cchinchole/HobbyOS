#include  <arch/i686/isr.h>
#include  <arch/i686/idt.h>
#include  <arch/i686/io.h>
#include  <arch/i686/panic.h>
#include  <libc/stdio.h>
#include  <libc/string.h>
#include  <drivers/vga.h>

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define MASTER_CMD 0x20
#define MASTER_DATA 0x21
#define SLAVE_CMD 0xA0
#define SLAVE_DATA 0xA1



void remap_pic(int offset1, int offset2)
{
    klog(LOG_SUCCESS, "Remapping PIC with commands: ", 0);
    kprintf("%x, %x\n", offset1, offset2);
    unsigned char a1, a2;
	a1 = pbin(MASTER_DATA);
	a2 = pbin(SLAVE_DATA);
	pbout(MASTER_CMD, ICW1_INIT | ICW1_ICW4);  
	pbout(SLAVE_CMD, ICW1_INIT | ICW1_ICW4);
	pbout(MASTER_DATA, offset1);      
	pbout(SLAVE_DATA, offset2);               
	pbout(MASTER_DATA, 4);                      
	pbout(SLAVE_DATA, 2);                
	pbout(MASTER_DATA, ICW4_8086);
	pbout(SLAVE_DATA, ICW4_8086);
	pbout(MASTER_DATA, a1); 
	pbout(SLAVE_DATA, a2);
}

void isr_install() {
    
    klog(LOG_INFO, "Initializing ISR.", 1);

    remap_pic(0x20, 0x28);
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);
    
    klog(LOG_INFO, "Initializing IRQ.", 1);

    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt();
    klog(LOG_SUCCESS, "IDT Done.", 1);
    asm volatile("sti");
    klog(LOG_SUCCESS, "Interrupts Enabled.", 1);
}

int register_interrupt_handler(uint8_t num, isr_t handler)
{
    interrupt_handlers[num] = handler;
    if(interrupt_handlers[num] == handler)
        return 1;
    else
        return 0;
    
}

void isr_handler(registers_t* r) {
isr_t handler = interrupt_handlers[(uint8_t)(r->int_no)];
	if (handler) {
		handler(r);
	} else {
		switch (r->int_no) {
			case 0:
				panic("[#0] Division by zero");
				break;

			case 1:
				panic("[#1] Debug exception");
				break;

			case 2:
				panic("[#2] Non-Maskable Interrupt");
				break;

			case 3:
				panic("[#3] Breakpoint exception");
				break;

			case 4:
				panic("[#4] Overflow exception");
				break;

			case 5:
				panic("[#5] Bound range exceeded");
				break;

			case 6:
				panic("[#6] Invalid opcode");
				break;

			case 7:
				panic("[#7] No math coprocessor");
				break;

			case 8:
				panic("[#8] Double fault exception");
				break;

			case 9:
				panic("[#9] Coprocessor segment overrun");
				break;

			case 10:
				panic("[#10] Invalid TSS");
				break;

			case 11:
			//	tty_puts("\n\n[error code: ");
			//	tty_puthex(r->err_code);
			//	tty_putc(']');
				panic("[#11] Segment not present");
				break;

			case 12:
				panic("[#12] Stack fault exception");
				break;

			case 13:
			//	tty_puts("\n\n[error code: ");
			//	tty_puthex(r->err_code);
			//	tty_putc(']');
				panic("[#13] General protection fault");
				break;

			case 14:
				panic("[#14] Page fault exception");
				break;

			case 15:
				panic("[#15] Reserved");
				break;

			case 16:
				panic("[#16] FPU error");
				break;

			case 17:
				panic("[#17] Alignment check exception");
				break;

			case 18:
				panic("[#18] Machine check exception");
				break;

			case 19:
				panic("[#19] SIMD Floating Point exception");
				break;

			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
				panic("[#20-31] Reserved");
				break;

			default:
				//warning("isr: null handler");
				break;
		}
	}

}


void irq_handler(registers_t* r)
{
    if(r->int_no >= 40)
        pbout(SLAVE_CMD, 0x20);
    pbout(MASTER_CMD, 0x20);
    if(interrupt_handlers[r->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
}