#include "mm/page.h"

// Our kernel will only use 4MB page frames. I just cannot be bothered to use the 4kib ones.
extern uint32_t kernel_end;

void switch_page_directory(page_directory_t* directory) {
    asm volatile("mov %0, %%cr3" : : "r" (directory));
}

void init_paging() {

    page_directory_t* kernel_directory = (page_directory_t*) 0x400000;  // Temp location for the kernel page directory
    kprintf("Kernel directory at 0x%x\n", kernel_directory);
    memset(kernel_directory, 0, sizeof(page_directory_t));

    if (kernel_directory == 0x0) {
        log_attribute(LOG_FATAL, "Could not allocate memory for the kernel page directory.");
        for (;;)
            asm("nop");
    }

    page_directory_entry_t* entry = &kernel_directory->entries[0];

    /*
    uint32_t present : 1;          // directory present in memory
    uint32_t writable : 1;         // directory is writable
    uint32_t user_accessible : 1;  // directory is accessible from user mode
    uint32_t page_write_through : 1;        // 4MB page directory flag
    uint32_t cache : 1;         // directory has been accessed (set by hardware)
    uint32_t accessed : 1;         // Reserved for future use
    uint32_t dirty : 1;           // Global directory (not flushed from TLB on task switch)
    uint32_t page_size : 1;        // Available for system software use
    uint32_t global : 1;        // Available for system software use
    uint32_t available : 3;        // Page table base address
    uint32_t page_attribute_table : 1;      // Reserved for future use
    uint32_t base_address_low : 8;
    uint32_t reserved : 1;        // Available for system software use
    uint32_t base_address_high : 10;    // 4MB directory base address
    */

    entry->present = 1;
    entry->writable = 1;
    entry->user_accessible = 0;
    entry->page_write_through = 0;
    entry->cache = 0;
    entry->accessed = 0;
    entry->dirty = 0;
    entry->reserved = 0;
    entry->page_size = 1;
    entry->global = 0;
    entry->available = 0;
    entry->page_attribute_table = 0;
    entry->base_address_low = 0;
    entry->reserved = 0;
    entry->base_address_high = 0;

    // TODO: make a way to easily map other addresses.

    register_interrupt_handler(14, page_fault_handler);

    // switch_page_directory(kernel_directory);

    load_page_directory(kernel_directory);

    for (;;);
}

// TODO: make this, if it receives a page fault because the page is not present, allocate a new page frame and map it to the address.
void page_fault_handler(registers_t* regs) {
    // uint32_t faulting_address;
    // asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    log_attribute(LOG_FATAL, "Page fault.");

    // kprintf("Page fault at 0x%x, faulting address 0x%x\n", regs->eip, faulting_address);

    for (;;)
        asm("nop");
}
