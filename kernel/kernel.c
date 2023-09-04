#include <stdint.h>

#include "drivers/tty/textmode.h"
#include "logging.h"
#include "mm/gdt.h"
#include "cpu/irq.h"
#include "misc/multiboot.h"

void kmain(multiboot_info_t* multiboot_header_pointer, void* stack_pointer, uint32_t bootloader_magic) {

    initialise_textmode();

    if (bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        log_attribute(LOG_FATAL, "Invalid boot-loader magic number");
    }

    initialise_gdt();
    log_attribute(LOG_INFO, "Loaded the GDT");

    initialise_idt();
    log_attribute(LOG_INFO, "Loaded the IDT");

    //enable_interrupts();
    log_attribute(LOG_INFO, "Enabled interrupts");

    kprintf("\nEnvironment information:\n");
    kprintf("Total memory: %dkb;", multiboot_header_pointer->mem_upper - multiboot_header_pointer->mem_lower);

    // --------|
    // Testing |
    // --------|



    // We need this here...
    for (;;);
}