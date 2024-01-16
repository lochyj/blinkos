#pragma once

#include <stdint.h>
#include <string.h>

#include "types.h"
#include "mm/kmalloc.h"
#include "cpu/irq.h"

// Define page directory entry structure (4MB pages)
typedef struct page_directory_entry {
    uint32_t present : 1;          // directory present in memory
    uint32_t writable : 1;         // directory is writable
    uint32_t user_accessible : 1;  // directory is accessible from user mode
    uint32_t page_size : 1;        // 4MB page directory flag
    uint32_t accessed : 1;         // directory has been accessed (set by hardware)
    uint32_t dirty : 1;            // directory has been written to (set by hardware)
    uint32_t reserved : 1;         // Reserved for future use
    uint32_t global : 1;           // Global directory (not flushed from TLB on task switch)
    uint32_t available : 3;        // Available for system software use
    uint32_t base_address : 20;    // 4MB directory base address
} page_directory_entry_t;

typedef struct page_directory {
    page_directory_entry_t entries[1024];  // 1024 page directory entries
} page_directory_t;


void switch_page_directory(page_directory_t* directory);

void init_paging();

void page_fault_handler(registers_t* regs);
