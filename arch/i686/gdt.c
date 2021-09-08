#include  <arch/i686/gdt.h>
#include <stdint.h>
#include <libc/stdio.h>

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install()
{
    klog(LOG_INFO, "Initializing GDT Table.", 1);
	gdtptr.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gdtptr.base = (uint32_t) &gdt;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	gdt_flush((uint32_t) &gdtptr);
    klog(LOG_SUCCESS, "Initialized GDT Table.", 1);
}
	