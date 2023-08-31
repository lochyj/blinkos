#pragma once

#include <stdint.h>

typedef struct {
    uint16_t segment_limit_low;
    uint16_t segment_base_low;
    uint8_t segment_base_middle;
    uint8_t accessed;
    uint8_t granularity;
    uint8_t segment_base_high;
} gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} gdt_pointer_t;

extern load_gdt(gdt_pointer_t*);

void set_gdt_descriptor(int descriptor, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

void initialise_gdt();
