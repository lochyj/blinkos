#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "drivers/tty/textmode.h"

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} error_type_t;

void kprintf(char* fmt, ...);
void log_attribute(error_type_t type, char* message);   // TODO: Make this format string so we can have more powerful logging
