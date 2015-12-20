#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

#include "multiboot.h"
#include "mb-memory-map.h"

#include <kernel/vga.h>

void kernel_early(void)
{	
	//terminal_initialize();
}

void kernel_main(multiboot_info_t* mbt, unsigned int magic)
{
	//printf("Hello, kernel World!\n");

	// check mode
	int cr0;
	asm ("movl %%cr0, %0;"
		: "=r"(cr0)
		: 
		: 
		);

	/*if((cr0 & 1) == 1) {
		printf("We're in protected mode\n");
	}
	else {
		printf("We're not in protected mode.\n");
	}

	unsigned long mem_size = 0;

	multiboot_memory_map_t* mmap = mbt->mmap_addr;
	while(mmap < mbt->mmap_addr + mbt->mmap_length) {
		if(mmap->type == 1)
			mem_size += mmap->length;

		printf("Memory part: 0x%lX, len: 0x%lX (%lu) KiB, type: %u\n", mmap->base_addr, (mmap->length), (uint64_t)(mmap->length/1024), mmap->type);

		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}

	printf("Available memory: %lu KiB\n", mem_size/1024);*/

	VGA_MEMORY[0] = make_vgaentry('O', COLOR_WHITE);
	VGA_MEMORY[1] = make_vgaentry('K', COLOR_WHITE);
	asm("xchgw %bx, %bx");
}
