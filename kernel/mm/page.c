#include "mm/page.h"

uint32_t flip_bit(uint32_t PTE, uint32_t offset) {
    if (offset > 31) {
        // Someone really fucked up here...
        return NULL;
    }

    // Xor will flip the bit at the desired location
    // that we shl 0b00...001 set by offset.
    return PTE ^ (0b1 << offset);
}

void create_page_table(uint32_t vaddr_begin) {
    // TODO
}
