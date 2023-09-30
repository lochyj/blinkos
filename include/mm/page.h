#pragma once

#include <stdint.h>
#include <string.h>

#include "types.h"
#include "mm/kmalloc.h"
#include "cpu/irq.h"

// 4MiB page frame size aligned address
#define ALIGN_TO_4MIB(addr) (((addr) + (4 * 1024 * 1024 - 1)) & ~(4 * 1024 * 1024 - 1))

typedef uint32_t pde_t;

// Page Table Entry (PTE) bit positions
enum {
    PRESENT = 0,
    READ_WRITE = 1,
    USER_SUPERVISOR = 2,
    WRITE_THROUGH = 3,
    CACHE_DISABLED = 4,
    ACCESSED = 5,
    DIRTY = 6,
    PAGE_SIZE = 7,
    GLOBAL = 8,
    AVAILABLE = 9,
    AVAILABLE_END = 11,
    PAGE_ATTRIBUTE_TABLE = 12,
    LOW_FRAME_BEGIN = 13,
    LOW_FRAME_END = 20,
    RSVD = 21, // I have no idea what this bit does...
    HIGH_FRAME_BEGIN = 22,
    HIGH_FRAME_END = 31
};

// Address of the page directory
extern void load_page_directory(uintptr_t);

uintptr_t map_kernel(void);

void page_fault_handler(registers_t* regs);
