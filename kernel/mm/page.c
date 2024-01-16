#include "mm/page.h"

// Our kernel will only use 4MB page frames. I just cannot be bothered to use the 4kib ones.
extern uint32_t kernel_end;

void switch_page_directory(page_directory_t* directory) {
    asm volatile("mov %0, %%cr3" : : "r" (directory));
}

void init_paging() {

    page_directory_t* kernel_directory = (page_directory_t*) dkmalloc(sizeof(page_directory_t), true);
    kprintf("Kernel directory at 0x%x\n", kernel_directory);
    memset(kernel_directory, 0, sizeof(page_directory_t));

    page_directory_entry_t* entry = &kernel_directory->entries[0];

    entry->present = 1;
    entry->writable = 1;
    entry->user_accessible = 0;
    entry->page_size = 1;
    entry->accessed = 0;
    entry->dirty = 0;
    entry->reserved = 0;
    entry->global = 1;
    entry->available = 0;
    entry->base_address = 0;

    // TODO: make a way to easily map other addresses.

    register_interrupt_handler(14, page_fault_handler);

    asm volatile("mov %0, %%cr3" : : "r" (kernel_directory));

    asm volatile("mov %cr0, %eax");
    asm volatile("or $0x80000000, %eax");
    asm volatile("mov %eax, %cr0");

}

// TODO: make this, if it receives a page fault because the page is not present, allocate a new page frame and map it to the address.
void page_fault_handler(registers_t* regs) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    kprintf("Page fault at 0x%x, faulting address 0x%x\n", regs->eip, faulting_address);

    for (;;)
        asm("nop");
}
