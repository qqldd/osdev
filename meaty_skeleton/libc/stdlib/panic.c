#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void panic(const char* str)
{
	// TODO: Add proper kernel panic.
	printf(str);
	while ( 1 ) { }
	__builtin_unreachable();
}
