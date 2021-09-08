
/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)
 
/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;
 
	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
 
	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
	kend = .;
}



/*
ENTRY(_loader)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
	
	. = 0x00100000;
	
	.setup : ALIGN(0x1000)
	{
		*(.multiboot)
		*(.setup)
	}
	
	. += 0xC0000000;
	
	start = .;
	
	.text : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}
	
	.rodata : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}
	
	.data : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}
	
	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		*(.stack)
	}
	
	end = .;
}
*/