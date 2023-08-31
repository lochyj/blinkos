#include <string.h>

void* memcpy(void* dest, const void* src, int count) {
	unsigned char* destC = (unsigned char*)dest;
	unsigned char* srcC = (unsigned char*)src;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = srcC[i];
	return dest;
}