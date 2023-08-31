#pragma once

#include "asm/i386/ports.h"

#define TEXTMODE_WIDTH 80
#define TEXTMODE_HEIGHT 25
#define TEXTMODE_DEFAULT_COLOR 0x07

void initialise_textmode_cursor(uint8_t cursor_start, uint8_t cursor_end);
void move_textmode_cursor(uint32_t x, uint32_t y);

uint16_t get_textmode_cursor_position();

void kputc(char character);
void kprint(char* string);

void initialise_textmode();