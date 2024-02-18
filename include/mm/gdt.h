#pragma once

#include <stdint.h>

#include "logging.h"

#define GDT_ENTRIES 6

typedef uint64_t gdt_t;

extern void load_gdt(uint32_t);

gdt_t create_gdt_descriptor(uint32_t base, uint32_t limit, uint16_t flag, uint8_t access);
void initialise_gdt();
