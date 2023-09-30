#include "mm/page.h"

// Our kernel will only use 4MB page frames. I just cannot be bothered to use the 4kib ones.

extern uint32_t kernel_end;

//Flip the bit at the desired offset for any paging related entry. Be that a PTE or a PDE.
void flip_bit(uint32_t* E, uint32_t offset) {
    if (offset > 31) {
        // Someone really fucked up here...
        return;
    }

    // Xor will flip the bit at the desired location
    // that we shl 0b00...001 set by offset.
    // We dont really need to do 0b1 we can just
    // do 1 but it makes more sense in context.
    *E = *E ^ (0b1 << offset);

    return;
}

// // There is a faster way of doing this refer to set_PSE_addr... TODO make it better...
// void set_addr(uint32_t* PTE, uint32_t addr) {
//     // remove the lower 12 bits
//     addr = (addr | 0b11111111111) ^ 0b11111111111;

//     // Clear the current address if there is one
//     *PTE = (*PTE | 0x1FFFFC000) ^ 0x1FFFFC000;

//     // Set the address :)
//     *PTE = *PTE | addr;

//     return;
// }

// PDE (Page Directory Entry) to load the addr (the address of the 4MiB PF)
void set_PSE_addr(uint32_t* PDE, uint32_t addr) {
    uint32_t addr_high = addr & 0b11111111110000000000000000000000;
    uint32_t addr_low = (addr & 0b00000000001111111100000000000000) >> 1;
    *PDE = *PDE & 0b00000000001000000001111111111111;
    *PDE = *PDE | (addr_high | addr_low);

    return;
}

uintptr_t map_kernel(void) {
    uint32_t kernel_begin = 0x100000;

    // The page directory...
    uint32_t* PD = (uint32_t*) kernel_end;

    // There shouldn't be anything after the kernel. So we can place the paging stuff there.
    // Make sure there isn't anything there. Even though there shouldn't be anything there
    memset(PD, 0, (sizeof(uint32_t) / sizeof(uint8_t)) * 0x1000);


    // 0x300 is the index of the first PDE that the kernel will be mapped to.
    // 0x300 * 4MiB = 0xC0000000
    uint32_t* PD_Kernel = PD + 0x300;

    // Alloc the kernel space.
    // We allocate the higher half of memory as being the kernel's.
    for (uint32_t i = 0; i < 256; i++) {

        flip_bit(PD_Kernel + (i * sizeof(pde_t)), PRESENT);           // The page is present in memory as the kernel is there. :)
        flip_bit(PD_Kernel + (i * sizeof(pde_t)), READ_WRITE);        // We want to be able to write to the kernel.
        flip_bit(PD_Kernel + (i * sizeof(pde_t)), USER_SUPERVISOR);   // We dont want user mode to be able to access the kernel.
        flip_bit(PD_Kernel + (i * sizeof(pde_t)), PAGE_SIZE);         // We are using 4MiB page frames.

        // Set the PDE to point to the 4MiB page frame
        // 0x400000 is 4MiB in hex.
        // We dont start at kernel_begin as it isnt aligned to 4MiB.
        // And we dont really care about mapping the kernel at precisely 0xC0000000.
        // As the kernel is already at 0x100000 this makes it simpler...
        set_PSE_addr(PD_Kernel + (i * sizeof(pde_t)), /*kernel_begin + */ (i * 0x400000));
    }

    // There aren't any other PDEs that we need to map for now as we aren't loading in anything below the kernel for now...

    return (uintptr_t) PD;

}

// TODO: make this, if it receives a page fault because the page is not present, allocate a new page frame and map it to the address.
void page_fault_handler(registers_t* regs) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    kprintf("Page fault at 0x%x, faulting address 0x%x\n", regs->eip, faulting_address);

    for (;;)
        asm("nop");
}
