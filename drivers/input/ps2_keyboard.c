#include "drivers/input/keyboard.h"

char sc_ascii_lower[] = {
    0x00, '~', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', 0x00, 0x00, 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', 0x00, 0x00, 'a', 's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l', ';', '\'', '`', 0x00, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0x00, 0x00, 0x00, ' '
};

enum {
    ERR = 0x00,
    ESCAPE = 0x01,
    BACKSPACE = 0x0E,
    ENTER = 0x1C,
    LEFT_CONTROL = 0x1D,
    LEFT_SHIFT = 0x2A,
    RIGHT_SHIFT = 0x36,
    LEFT_ALT = 0x38,
    SPACE = 0x39,
    CAPS_LOCK = 0x3A,
};

// In the future we want to add all of the inputs to a buffer; this will then get fed into any program that is listening for input.
void keyboard_handler(registers_t* registers) {
    uint8_t scancode = inb(0x60);

    if (scancode == LEFT_SHIFT + 0x80 || scancode == RIGHT_SHIFT + 0x80 || scancode == LEFT_CONTROL + 0x80 || scancode == LEFT_ALT + 0x80) {
		// nothing to see here...
	} else if(scancode > 0x80) {
		return;
	}

    switch (scancode) {
        case ENTER:
            kprintf("\n");
            return;
        case BACKSPACE:
            kprintf("\b");
            return;
        default:
            break;
    }

    char letter = sc_ascii_lower[(int) scancode];

    kprintf("%c", letter);
}

void register_keyboard_driver() {
    register_interrupt_handler(IRQ1, keyboard_handler);
}