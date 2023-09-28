#include "mm/page.h"

//Flip the bit at the desired offset for any paging related entry. Be that a PTE or a PDE.
void flip_bit(uint32_t* E, uint32_t offset) {
    if (offset > 31) {
        // Someone really fucked up here...
        return NULL;
    }

    // Xor will flip the bit at the desired location
    // that we shl 0b00...001 set by offset.
    *E = *E ^ (0b1 << offset);
}

void set_addr(uint32_t* PTE, uint32_t addr) {
    // remove the lower 12 bits
    addr = (addr | 0b11111111111) ^ 0b11111111111;

    // Clear the current address if there is one
    *PTE = (*PTE | 0x1FFFFC000) ^ 0x1FFFFC000;

    // Set the address :)
    *PTE = *PTE | addr;

    return;
}

uint32_t create_page_table(uint32_t vaddr_begin) {
    // Stupidly (dumb kernel malloc()) allocate the memory for now...
    pte_t* phys = (pte_t*) dkmalloc(sizeof(pte_t) * 1024, 4096);

    // Because we dont have a dkcalloc():
    // sizeof(pte_t) / sizeof(uint8_t) will always be 4.
    // This is because pte_t is 32bit and uint8_t is 8 bit.
    // We need this because memset() takes a char value so this is necessary.
    memset(phys, 0b00000000, (sizeof(uint8_t) * (sizeof(pte_t) / sizeof(uint8_t))) * 1024);

    for (uint32_t i = 0; i < 1024; i++) {
        uint32_t PTE;

        flip_bit(&PTE, PRESENT);

        set_addr(&PTE, dkmalloc(4096, 4096));

        phys[i] = PTE;

    }

    return 0;
}

void map_kernel(uint32_t vaddr_from) {
    uint32_t kernel_begin = 0x100000;

    // TODO;

}
