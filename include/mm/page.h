#pragma once

#include <stdint.h>

#include "types.h"
#include "mm/kmalloc.h"

// Page Table Entry (PTE) bit positions
enum {
    PRESENT = 0,
    READ_WRITE = 1,
    USER_SUPERVISOR = 2,
    WRITE_THROUGH = 3,
    CACHE_DISABLED = 4,
    ACCESSED = 5,
    DIRTY = 6,
    PAGE_ATTRIBUTE_TABLE = 7,
    GLOBAL = 8,
    AVAILABLE = 9,
    AVAILABLE_END = 11,
    FRAME = 12,
    FRAME_END = 31
};

// Address of the page directory
extern void load_page_directory(uintptr_t);

// TODO: remove this from here... We only need this here for testing...
uint32_t flip_present(uint32_t PTE);
