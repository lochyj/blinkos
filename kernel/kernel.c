#include <stdint.h>

#include "drivers/tty/textmode.h"
#include "logging.h"
#include "mm/gdt.h"
#include "cpu/irq.h"

// TODO: Get the multiboot.h file from the GRUB source code
void kmain(void* multiboot_header_pointer, void* stack_pointer, uint32_t header_magic) {

    initialise_textmode();

    if (header_magic != 0x2BADB002) {
        log_attribute(LOG_FATAL, "Invalid multiboot header magic number");
    }

    initialise_gdt();
    log_attribute(LOG_INFO, "Loaded the GDT");

    initialise_idt();
    log_attribute(LOG_INFO, "Loaded the IDT");

    enable_interrupts();
    log_attribute(LOG_INFO, "Enabled interrupts");


    // --------|
    // Testing |
    // --------|

    // Testing the kprintf function:
    kprintf("Hello, %s!\n", "world");
    kprintf("The number %d is equal to %x in hex!\n", 42, 42);
    kprintf("The number %d is equal to %b in binary!\n", 42, 42);
    kprintf("The number %d is equal to %o in octal!\n", 42, 42);

    // Testing the logging functions:
    log_attribute(LOG_INFO, "This is an info message");
    log_attribute(LOG_WARNING, "This is a warning message");
    log_attribute(LOG_ERROR, "This is an error message");
    log_attribute(LOG_FATAL, "This is a fatal message");

    // We need this here...
    for (;;);
}