#pragma once

#include <stdint.h>

#include "asm/i386/ports.h"
#include "logging.h"

enum {
    APM,
    ACPI,
    BOCHS,
    QEMU,
    VIRTUALBOX
};

void shutdown(int hardware);
