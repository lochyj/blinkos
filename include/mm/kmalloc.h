#pragma once

#include <stdint.h>
#include <stdbool.h>

void init_heap(uint32_t heap_begin);

uint32_t dkmalloc(uint32_t size, bool align);

// TODO:
// - kmalloc
