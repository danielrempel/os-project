# Multiboot header constants
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Multiboot header itself
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Initial stack
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.set KERNEL_VIRTUAL_BASE, 0xC0000000 # 3GB
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22) # Page directory index of kernel's 4MB PTE.

.section .data
.align 0x1000
BootPageDirectory:
	.byte 0x83
	.byte 0
	.byte 0
	.byte 0
	.fill (KERNEL_PAGE_NUMBER - 1), 4, 0x0
	.byte 0x83
	.byte 0
	.byte 0
	.byte 0
	.fill (1024 - KERNEL_PAGE_NUMBER - 1), 4, 0x0

.section .text
.global start 
start: # The kernel entry point.

	# Load page directory address
	mov $(BootPageDirectory - KERNEL_VIRTUAL_BASE),%ecx
	mov %ecx,%cr3

	# Enable 4MB pages
	mov %cr4,%ecx
	or $0x10, %ecx
	mov %ecx,%cr4

	# Enable paging
	mov %cr0,%ecx
	or $0x80000000, %ecx
	mov %ecx, %cr0

	lea (high_start), %ecx
	jmp %ecx

	# Hang if kernel's main unexpectedly returns.
	cli
	hlt

high_start:
	# invalidate first page // 0x100,000 -> 0x100,000
	movl $0x00000000, (BootPageDirectory)
	invlpg (0)

	movl $stack_top, %esp
	
	# Push multiboot data pointer to stack
	# TODO: check that the address is OK
	add $0xC0000000, %ebx
	pushl %ebx

	# Kernel initialisation
	call kernel_early

	# Global constructors
	# GCC's additions for CPP?
	call _init

	# Kernel!
	call kernel_main

.Lhang:
	jmp .Lhang
