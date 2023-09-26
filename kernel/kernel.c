#include <stdint.h>

#include "drivers/tty/textmode.h"
#include "logging.h"
#include "mm/gdt.h"
#include "cpu/irq.h"
#include "misc/multiboot.h"
#include "drivers/input/keyboard.h"
#include "cpu/power.h"
#include "mm/page.h"

void kmain(multiboot_info_t* multiboot_header_pointer, void* stack_pointer, uint32_t bootloader_magic) {

    if (bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        initialise_textmode();
        log_attribute(LOG_FATAL, "Invalid boot-loader magic number");

        // Just assume we are using QEMU for now...
        shutdown(QEMU);

        return; // This will never return... Hopefully.
    }

    initialise_textmode();

    initialise_gdt();
    log_attribute(LOG_INFO, "Loaded the GDT");

    initialise_idt();
    log_attribute(LOG_INFO, "Loaded the IDT");

    register_keyboard_driver();
    log_attribute(LOG_INFO, "Loaded the keyboard driver");

    enable_interrupts();
    log_attribute(LOG_INFO, "Enabled interrupts");

    kprintf("\nEnvironment information:\n");
    kprintf("Total memory: %dkb;\n", multiboot_header_pointer->mem_upper - multiboot_header_pointer->mem_lower);

    // --------|
    // Testing |
    // --------|

    uint32_t a = 0x00000001;

    uint32_t b = flip_present(a);

    kprintf("A: %d; B: %d\n", a, b);

    // We need this here...
    for (;;)
        asm("nop");
}