#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include  <libc/stdio.h>
#include  <libc/string.h>
#include  <drivers/vga.h>
 


static int kprint(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (put_char(bytes[i]) == EOF)
			return 0;
	return 1;
}

void kprint_hex(uint32_t n)
{
    int tmp;

    put_char('0');
    put_char('x');

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            put_char((tmp-0xA+'a'));
        }
        else
        {
            noZeroes = 0;
            put_char( (tmp+'0'));
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        put_char ( (tmp-0xA+'a'));
    }
    else
    {
        put_char ((tmp+'0'));
    }

}

void kprint_dec(uint32_t n)
{
    if (n == 0)
    {
        put_char('0');
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    kprint(c2, strlen(c2));
}
 
int kprintf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (kprint(format, amount) == 0)
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(str, len))
				return -1;
			written += len;
		} else if(*format == 'd') {
            format++;
		    int dec = va_arg(parameters, int);
			size_t len = sizeof(int);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			kprint_dec(dec);
			written += len;
        } else if(*format == 'x') {
            format++;
		    int dec = va_arg(parameters, int);
			size_t len = sizeof(int);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			kprint_hex(dec);
			written += len;
        } 
        else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!kprint(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}


void klog(int logType, char* message, int endLine)
{
	switch(logType)
	{
		case LOG_SUCCESS:
			set_attribute(GREEN);
			kprintf("[ Success ] ");
			set_attribute(WHITE);
			kprintf(message);
		break;
		case LOG_ERROR:
			set_attribute(RED);
			kprintf("[ Error ] ");
			set_attribute(WHITE);
			kprintf(message);
		break;
		case LOG_INFO:
			set_attribute(YELLOW);
			kprintf("[ Info ] ");
			set_attribute(WHITE);
			kprintf(message);
		break;

	}

	if(endLine==1)
		kprintf("\n");
}