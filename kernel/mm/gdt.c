#include "mm/gdt.h"

#define GDT_ENTRIES 6

gdt_entry_t gdt[GDT_ENTRIES];
gdt_pointer_t gdt_pointer;

void set_gdt_descriptor(int descriptor, uintptr_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[descriptor].segment_limit_low   = limit & 0xFFFF;
    gdt[descriptor].segment_base_low    = base & 0xFFFF;
    gdt[descriptor].segment_base_middle = (base >> 16) & 0xFF;
    gdt[descriptor].accessed            = access;
    gdt[descriptor].granularity         = (limit >> 16) & 0x0F;
    gdt[descriptor].granularity         |= (granularity & 0xF0);
    gdt[descriptor].segment_base_high   = (base >> 24) & 0xFF;
}

void initialise_gdt() {
    gdt_pointer.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gdt_pointer.base = (uint32_t) &gdt;

    set_gdt_descriptor(0, 0, 0, 0, 0);  // Empty descriptor. See Figure 5-5 at https://css.csail.mit.edu/6.858/2019/readings/i386.pdf
    set_gdt_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   // Code
    set_gdt_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   // Data
    //set_gdt_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);   // User mode code - For the future
    //set_gdt_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);   // User mode data - For the future

    load_gdt(&gdt_pointer);
}