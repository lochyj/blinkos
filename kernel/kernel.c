#include <stdint.h>

#include "drivers/tty/textmode.h"
#include "mm/gdt.h"

void kmain(void* multiboot_header_pointer, void* stack_pointer, uint32_t header_magic) {

    initialise_textmode();

    if (header_magic != 0x2BADB002) {
        kprint("[ ERROR ] Invalid multiboot header magic number\n");
    }

    kprint("Hello, there!\n");

    initialise_gdt();

}