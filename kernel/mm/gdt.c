#include "mm/gdt.h"

gdt_t gdt[6];

void register_gdt_descriptor(uint8_t vector, uint32_t base, uint32_t limit, uint8_t access, uint16_t flag) {
    gdt_t descriptor = 0;

    // Extract the last byte (8 bits) of the base address value and place it at the very end of the descriptor (we will move it there in a moment).
    descriptor |= (((1 << 8) - 1) << 24) & base;
    descriptor <<= 8;   // Shift over by 8 bits

    descriptor |= flag; // Place the 4 flag bits in the last half of the second last byte.
    descriptor <<= 4;   // ...

    // Extract the 16th to the 19th bit in the limit address and place it in the second half of the second last byte in the descriptor.
    descriptor |= (((1 << 4) - 1) << 16) & limit;
    descriptor <<= 4;   // ...

    descriptor |= access;
    descriptor <<= 8;

    // Extract the 16th to the 23rd bit in the base address.
    descriptor |= (((1 << 8) - 1) << 16) & base;
    descriptor <<= 8;

    descriptor |= ((1 << 16) - 1) & base;
    descriptor <<= 16;

    descriptor |= ((1 << 16) - 1) & limit;

    gdt[vector] = descriptor;

}

void initialise_gdt() {
    register_gdt_descriptor(0, 0x0, 0x0, 0x0, 0x0);      // Null descriptor
    register_gdt_descriptor(1, 0x0, 0xFFFFF, 0x9a, 0xC); // Kernel mode code segment
    register_gdt_descriptor(2, 0x0, 0xFFFFF, 0x92, 0xC); // Kernel mode data segment
    register_gdt_descriptor(3, 0x0, 0xFFFFF, 0xFA, 0xC); // User mode code segment
    register_gdt_descriptor(4, 0x0, 0xFFFFF, 0xF2, 0xC); // User mode data segment

    // TODO:
    //register_gdt_descriptor(0, ); // Task state segment

    // Set the limit   |                       limit                           |   |addr| and the address of the gdt.
    uint64_t gdt_ptr;
    gdt_ptr = ((sizeof(gdt_t) * GDT_ENTRIES) - 1) << sizeof(uint32_t);
    gdt_ptr |= (uint32_t) (&gdt);

    load_gdt(&gdt_ptr);
}
