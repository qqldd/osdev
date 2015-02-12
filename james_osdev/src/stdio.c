#include "stdio.h"
#include <stdarg.h>
#include "monitor.h"
#include <stdbool.h>

int putchar(char c){
    monitor_put(c);
    return 0;
}

static void print(const char* data, u32int data_length)
{
    for ( u32int i = 0; i < data_length; i++ )
        putchar((int) ((const unsigned char*) data)[i]);
}

int printf(const char* __restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    u32int amount;
    bool rejected_bad_specifier = false;
    while ( *format != '\0' )
    {
        if ( *format != '%' )
        {
            print_c:
            amount = 1;
            while ( format[amount] && format[amount] != '%' )
                amount++;
            print(format, amount);
            format += amount;
            written += amount;
            continue;
        }
        const char* format_begun_at = format;
        if ( *(++format) == '%' )
            goto print_c;
        if ( rejected_bad_specifier )
        {
        incomprehensible_conversion:
            rejected_bad_specifier = true;
            format = format_begun_at;
            goto print_c;
        }
        if ( *format == 'c' )
        {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            print(&c, sizeof(c));
        }
        else if ( *format == 's' )
        {
            format++;
            const char* s = va_arg(parameters, const char*);
            print(s, strlen(s));
        }
        else if (*format == 'u' )
        {
            format++;
            u32int u = (u32int) va_arg(parameters, int);
            monitor_write_dec(u);
            
        }
        else if (*format == 'x')
        {
            format++;
            u32int x = (u32int) va_arg(parameters, int);
            monitor_write_hex(x);
        }
        else
        {
            goto incomprehensible_conversion;
        }
    }
    va_end(parameters);
    return written;
}

int puts(const char* string){
    return printf("%s\n", string);
}
