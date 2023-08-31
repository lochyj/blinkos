#include <string.h>

void* memset(void* dest, unsigned char val, int count) {
	unsigned char* destC = (unsigned char*)dest;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = val;
	return dest;
}