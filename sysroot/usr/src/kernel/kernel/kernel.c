#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <kernel/tty.h>

#include "multiboot.h"
#include "mb-memory-map.h"

void kernel_early(void)
{
	terminal_initialize();
}

void kernel_main(multiboot_info_t* mbt, unsigned int magic)
{
	printf("Hello, kernel World!\n");
	
	// check mode
	int cr0;
	asm ("movl %%cr0, %0;"
		: "=r"(cr0)
		: 
		: 
		);
		
	if((cr0 & 1) == 1) {
		terminal_writestring("We're in protected mode\n");
	}
	else {
		terminal_writestring("We're not in protected mode.\n");
	}
	
	unsigned long long mem_size = 0;
	
	multiboot_memory_map_t* mmap = mbt->mmap_addr;
	while(mmap < mbt->mmap_addr + mbt->mmap_length) {
		if(mmap->type == 1)
			mem_size += mmap->length;
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}
	terminal_writestring("Available memory: ");
	unsigned char *ptr = &mem_size;
	mem_size /= 1024; // to KiBs
	
	// convert ordinary number (represented via bytes) to weird magical
	// representation which can be printed as decimal
	unsigned long long decimal = 0, modifier = 1;
	while(mem_size > 0) {
		// ptr points to lsp
		// ptr+i to current beginning from hsp
		decimal += (mem_size%10) * modifier;
		modifier *= 16;
		mem_size/=10;
	}
	
	ptr = &decimal;
	for(int i=sizeof(unsigned long long)+1; i>0; i-=1) {
		terminal_putchar("0123456789ABCDEF"[*(ptr+i-1) / 16]);
		terminal_putchar("0123456789ABCDEF"[*(ptr+i-1) % 16]);
	}
	terminal_writestring(" KiB\n");
}
