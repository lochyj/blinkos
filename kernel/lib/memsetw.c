#include <string.h>

unsigned short* memsetw(unsigned short* dest, unsigned short val, int count) {
	int i;
	for (i = 0; i < count; i++)
		dest[i] = val;
	return dest;
}