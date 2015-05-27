
typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned long base_addr;
// You can also use: unsigned long long int base_addr; if supported.
//	unsigned int length_low,length_high;
	unsigned long length;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;
