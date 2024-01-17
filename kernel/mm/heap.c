#include "mm/kmalloc.h"

uint32_t heap_begin;
uint32_t heap_head;

void init_heap(uint32_t heap_base) {
    heap_begin = heap_base;
    heap_head = heap_begin;
}

// 4MiB page frame size aligned address
#define ALIGN_TO_4MiB(addr) \
    (((addr) + 0x3FFFFF) & ~0x3FFFFF)

// The dumb kmalloc. A better version is underway.
uint32_t dkmalloc(uint32_t size, bool align) {

    if (align) {

        size = ALIGN_TO_4MiB(size);

    }

    uint32_t alloc_base = ALIGN_TO_4MiB(heap_head);

    heap_head += size;

    return alloc_base;
}
