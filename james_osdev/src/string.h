#ifndef STRING_H
#define STRING_H

#include "common.h"

size_t strlen(const char*);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memset(void*, int, size_t);
void* memmove(void*, const void*, size_t);
int memcmp(const void*, const void*, size_t);

int strcmp(char* str1, char* str2);
char* strcpy(char* dst, const char* src);
char* strcat(char* dst, const char* src);

#endif
