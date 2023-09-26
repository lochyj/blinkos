#pragma once

#include <stdint.h>

#include "asm/i386/page.h"

// Address of the page directory
extern void load_page_directory(uintptr_t);
