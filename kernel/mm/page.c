#include "mm/page.h"

uint32_t flip_present(uint32_t PTE) {
    uint32_t x = PTE | 0x00000001;  // We use hex because its smaller.

    x = ~x;                         // Flip the bits

    return x ? PTE | x : PTE & x;   // If x is 0, clear the present bit with PTE & x, else set the present bit with PTE | x.

}
