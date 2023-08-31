#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef unsigned long size_t;

typedef uint32_t physaddr_t;

typedef struct {
   bool display_mode;   // true = text mode, false = graphics / fb mode

   uint32_t mem_lower;
   uint32_t mem_upper;

} kernel_info_t;
