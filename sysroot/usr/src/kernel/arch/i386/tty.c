#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
	if(c != '\n') {
		
		if( !((c>='a' && c<='z')||(c>='A' && c<='Z')||
			(c>='0'&&c<='9')||
			(c=='\\')||
			(c==' ')||
			(c=='\'')||
			(c==':')||
			(c=='_')
			
			) ) {
			terminal_putchar('\\');
			terminal_putchar("0123456789ABCDEF"[c / 16]);
			terminal_putchar("0123456789ABCDEF"[c % 16]);
		} else {		
		
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if ( ++terminal_column == VGA_WIDTH )
			{
				terminal_column = 0;
				if(terminal_row == VGA_HEIGHT-1) {
					for(int i=1; i<VGA_HEIGHT; i+=1) {
						// memmove
						for(int j=0; j<VGA_WIDTH; j+=1)
							terminal_buffer[(i-1)*VGA_WIDTH+j] = terminal_buffer[i*VGA_WIDTH+j];
					}
					for(int j=0; j<VGA_WIDTH; j+=1)
						terminal_putentryat(' ', terminal_color, j, terminal_row);
					terminal_row = VGA_HEIGHT-1;
				}
				else
					terminal_row += 1;
			}
			
		}
	}
	else {
		terminal_column = 0;
		
		if(terminal_row == VGA_HEIGHT-1) {
			for(int i=1; i<VGA_HEIGHT; i+=1) {
				// memmove
				for(int j=0; j<VGA_WIDTH; j+=1)
					terminal_buffer[(i-1)*VGA_WIDTH+j] = terminal_buffer[i*VGA_WIDTH+j];
			}
			for(int j=0; j<VGA_WIDTH; j+=1)
				terminal_putentryat(' ', terminal_color, j, terminal_row);
			terminal_row = VGA_HEIGHT-1;
		}
		else
			terminal_row += 1;
	}
}

void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
