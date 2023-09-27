#pragma once

#include <stdint.h>

void init_heap(uint32_t heap_begin);

uintptr_t dkmalloc(uint32_t size, uint32_t align);

// TODO:
// - kmalloc
