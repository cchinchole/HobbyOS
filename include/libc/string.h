#ifndef STRING_H
#define STRING_H
#include <stddef.h>
#include <stdint.h>

void itoa(int n, char* str);
void reverse(char* s);
void append(char* s, char n);
int strcmp(char* s1, char* s2);
int memcmp(const void*, const void*, size_t);
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size);

//void memory_copy(uint8_t *source, uint8_t *dest, int nbytes);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char* str);
char *strcpy(char *dest, const char *src);


#endif
