#include "mm/kmalloc.h"

uint32_t heap_begin;
uint32_t heap_head;

void init_heap(uint32_t heap_base) {
    heap_begin = heap_base;
    heap_head = heap_begin;
}

// The dumb kmalloc.
// TODO: test if this works. It should in theory. (it may not lol)
uint32_t dkmalloc(uint32_t size, uint32_t align) {
    uint32_t size_aligned;

    if (heap_head % align == 0) {

        size_aligned = size;

    } else {
        // There is definitely a better way of doing this...
        // get the next aligned address from heap_head that aligns with align
        while (size_aligned + heap_head % align != 0) {
            size_aligned += 1;
        }

        size_aligned += size;
    }

    uint32_t alloc_base = heap_head;

    heap_head += size;

    return alloc_base;
}
