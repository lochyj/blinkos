#include "drivers/tty/textmode.h"

// TODO: cleanup the magic numbers in the code

volatile char* video_memory = (volatile char*) 0xB8000;

void initialise_textmode_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void move_textmode_cursor(uint32_t x, uint32_t y) {
    uint32_t position = y * TEXTMODE_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

uint16_t get_textmode_cursor_position() {
    uint16_t position = 0;

    outb(0x3D4, 0x0F);
    position |= inb(0x3D5);

    outb(0x3D4, 0x0E);
    position |= ((uint16_t) inb(0x3D5)) << 8;

    return position;
}


// TODO: clean up the code
void kputc(char character) {
    uint16_t cursor_position = get_textmode_cursor_position();

    uint32_t x, y;

    x = cursor_position % TEXTMODE_WIDTH;
    y = cursor_position / TEXTMODE_WIDTH;

    if (character == '\n') {

        move_textmode_cursor(0, y + 1);
        return;

    } else if (character == '\r') {

        move_textmode_cursor(0, y);
        return;

    } else if (character == '\b') {

        if (x == 0 && y == 0)
            return;

        if (x == 0) {
            move_textmode_cursor(TEXTMODE_WIDTH - 1, y - 1);
        } else {
            move_textmode_cursor(x - 1, y);
        }

        kputc(' ');
        move_textmode_cursor(x - 1, y);

        return;

    } else if (character == 0x00) {
        return;
    }

    video_memory[cursor_position * 2] = character | (TEXTMODE_DEFAULT_COLOR << 8);
    move_textmode_cursor(x + 1, y);

}

void initialise_textmode() {
    initialise_textmode_cursor(0, 15);
}

void kprint(char* string) {
    uint32_t i = 0;

    while (string[i] != 0) {
        kputc(string[i]);
        i++;
    }
}