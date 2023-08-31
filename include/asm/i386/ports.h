#pragma once

#include <stdint.h>

// Read a byte from the specified port
extern uint8_t inb(uint16_t);

// Write a byte to the specified port
extern void outb(uint16_t, uint8_t);
