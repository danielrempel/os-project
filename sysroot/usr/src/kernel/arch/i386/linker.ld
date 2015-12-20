OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

physical = 0x100000;
offset = 0xC0000000;
virtual = physical + offset;

SECTIONS
{	
	. = virtual; /* the code should be relocated to 3GB + 1MB */
	
	.text ALIGN(4K) : AT(ADDR(.text)-offset)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata)-offset) 
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT(ADDR(.data)-offset)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT(ADDR(.bss)-offset)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
}
