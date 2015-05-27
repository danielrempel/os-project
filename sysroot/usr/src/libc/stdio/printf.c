#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*
sizeof(long) == 8
sizeof(short) == 2
sizeof(int) == 4
sizeof(char) == 1
sizeof(int64_t) == 8
sizeof(int32_t) == 4
sizeof(int16_t) == 2
sizeof(int8_t) == 1
*/

static void print(const char* data, size_t data_length)
{
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

/**
 * 
 * prints an integer in decimal representation
 * 
 * @param intSelf signless original integer
 * @param sign - represents a sign (-1,+1)
 * @param size - sizeof of original datatype (1,2,4,8) to pad with right amount of zeroes
 * @param zeroPad - (1,0) marks whether to pad or not to pad the integer printed with zeroes
 * 
 * @return count of chars printed
 * */
static int32_t print_dec(const uint64_t intSelf, int8_t sign, int8_t size, int8_t zeroPad) {
	int32_t written = 0;
	
	uint64_t divider = 10000000000000000000U;
	uint64_t intCopy = intSelf;
	int8_t nonzero = zeroPad;
	int8_t current = 0;
	
	if(sign == -1)
		putchar('-');
	
	while(divider > 0) {
		current = (intCopy / divider);
		
		if(current != 0) {
			putchar("0123456789"[current]);
			written += 1;
			if(nonzero == 0)
				nonzero = 1;
		}
		if((current == 0) && (nonzero == 1)) {
			putchar("0123456789"[current]);
			written += 1;
		}
		
		intCopy %= divider;
		divider /= 10;
	}
	
	return written;
}

/**
 * 
 * prints an integer in hexademical representation
 * 
 * @param intSelf signless original integer
 * @param size - sizeof of original datatype (1,2,4,8) to pad with right amount of zeroes
 * @param zeroPad - (1,0) marks whether to pad or not to pad the integer printed with zeroes
 * 
 * @return count of chars printed
 * */
static int32_t print_hex(const uint64_t intSelf, int8_t size, int8_t uppercase) {
	int32_t written = 0;
	uint64_t divider = 0x1000000000000000U;
	uint64_t intCopy = intSelf;
	
	int8_t current = 0;
	while(divider > 0) {
		current = (intCopy / divider);
		
		if(current != 0) {
			if(uppercase>0)
				putchar("0123456789ABCDEF"[current]);
			else
				putchar("0123456789abcdef"[current]);
			written += 1;
		}
		
		intCopy %= divider;
		divider /= 16;
	}
	
	return 0;
}

/**
 * 
 * printf function
 * 
 * 
 * */
int printf(const char* restrict format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	int32_t written = 0;

	bool convStarted = 0;
	bool convert = 0;
	
	int8_t zeroPad = 0;
	
	int8_t uppercaseHex = 0;
	
	enum types { t_char, t_short, t_int, t_long } currentFlag = t_int;
	enum conversions { c_int, c_uint, c_uint_hex, c_char, c_str, c_ptr } currentConv = c_int;
	
	#define signf(x) (x>=0 ? 1 : -1)
	#define abs(x) (x<0 ? -x : x)

	while ( *format != '\0' )
	{
		if(convStarted == 0) {
			// chars
			if(*format != '%') {
				print(format, 1);
				written += 1;
			}
			else {
				convStarted = 1;
				currentConv = c_int;
				currentFlag = t_long;
				zeroPad = 0;
				uppercaseHex = 0;
			}
		} else {
			switch(*format) {
				case 'd':
				case 'i':
					currentConv = c_int;
					convert = 1;
				break;
				case 'o':
					// unimplemented octal conversion
				case 'u':
					currentConv = c_uint;
					convert = 1;
				break;
				case 'x':
					currentConv = c_uint_hex;
					convert = 1;
					uppercaseHex = 0;
				break;
				case 'X':
					currentConv = c_uint_hex;
					convert = 1;
					uppercaseHex = 1;
				break;
				case 'c':
					currentConv = c_char;
					convert = 1;
				break;
				case 's':
					currentConv = c_str;
					convert = 1;
				break;
				case 'p':
					currentConv = c_ptr;
					convert = 1;
				break;
				
				case 'h':
					currentFlag = t_short;
					if( *(format+1) == 'h' ) {
						format ++;
						currentFlag = t_char;
					}
				break;
				case 'l':
					currentFlag = t_long;
					if( *(format+1) == 'l' ) {
						format ++;
					}
				break;
				
				case '0':
					zeroPad = 1;
				break;
				default:
					convStarted = 0;
			}
			
			if(convert == 1) {
				convert = 0;
				convStarted = 0;
				
				uint64_t param = 0;
				int64_t tmp = 0;
				int8_t sign = 1;
				int8_t size = 0;
				switch (currentConv) {
					case c_int:
						switch(currentFlag) {
							case t_long: {
								tmp = (int64_t) va_arg(parameters, int64_t);
								param = abs(tmp);
								sign = signf(tmp);
								size = 8;
								}
							break;
							case t_char:
							case t_short:
							case t_int:
							default: {
								tmp = (int32_t) va_arg(parameters, int32_t);
								param = abs(tmp);
								sign = signf(tmp);
								size = 4;
							}
						}
						print_dec(param,sign,size,zeroPad);
					break;
					case c_uint_hex:
					case c_uint:
						switch(currentFlag) {
							case t_long: {
								param = (uint64_t) va_arg(parameters, uint64_t);
								sign = 1;
								size = 8;
								}
							break;
							case t_char:
							case t_short:
							case t_int:
							default: {
								param = (uint32_t) va_arg(parameters, uint32_t);
								sign = 1;
								size = 4;
							}
						}
						if(currentConv == c_uint_hex)
							print_hex(param,size,uppercaseHex);
						else
							print_dec(param,sign,size,zeroPad);
					break;
					case c_char: {
						char c = (char) va_arg(parameters, int32_t /* char promotes to int */);
						print(&c, sizeof(c));
					}
					break;
					case c_str: {
						const char* s = va_arg(parameters, const char*);
						print(s, strlen(s));
					}
					break;
					case c_ptr: {
						uint32_t ptr = (uint32_t) va_arg(parameters, uint32_t);
						print_hex(ptr,sizeof(uint32_t),uppercaseHex);
					}
					break;
				}
			}
		}
		
		format ++;
	}

	#undef abs
	#undef sign

	va_end(parameters);

	return written;
}
