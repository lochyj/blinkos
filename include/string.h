#pragma once

#include "types.h"

void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, unsigned char val, int count);
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
char* strcat(char *s, const char *append);
int strcmp(const char *s1, const char *s2);
char* strcpy(char *to, const char *from);
size_t strlcat(char *dst, const char *src, size_t dsize);
size_t strlen(const char *str);