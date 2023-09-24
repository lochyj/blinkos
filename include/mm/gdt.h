#pragma once

#include <stdint.h>

#define GDT_ENTRIES 6

typedef struct {
    uint16_t segment_limit_low;
    uint16_t segment_base_low;
    uint8_t segment_base_middle;
    uint8_t accessed;
    uint8_t granularity;
    uint8_t segment_base_high;
} gdt_entry_t;  // Through testing I dont think this needs to be packed. Please correct me if I'm wrong.

typedef struct {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed)) gdt_pointer_t;    // However, this one definitely does...

extern void load_gdt(gdt_pointer_t*);

void set_gdt_descriptor(int32_t descriptor, uintptr_t base, uint32_t limit, uint8_t access, uint8_t granularity);

void initialise_gdt();
