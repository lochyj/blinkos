#include <stdint.h>

#include "drivers/tty/textmode.h"
#include "logging.h"
#include "mm/gdt.h"
#include "cpu/irq.h"
#include "misc/multiboot.h"
#include "drivers/input/keyboard.h"
#include "cpu/power.h"
#include "mm/page.h"
#include "mm/kmalloc.h"

extern uint32_t kernel_end;

#define ALIGN_TO_4MIB(addr) (((addr) + (4 * 1024 * 1024 - 1)) & ~(4 * 1024 * 1024 - 1))

void kmain(multiboot_info_t* multiboot_header_pointer, void* stack_pointer, uint32_t bootloader_magic) {

    if (bootloader_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        // Just assume we are using QEMU for now...
        shutdown(QEMU);

        return; // This will never return... Hopefully.
    }

    uint32_t heap_location = kernel_end;

    initialise_textmode();

    initialise_gdt();
    log_attribute(LOG_INFO, "Loaded the GDT");

    initialise_idt();
    log_attribute(LOG_INFO, "Loaded the IDT");

    init_heap(heap_location);
    log_attribute(LOG_INFO, "Loaded the Heap at the end of the kernel");

    register_keyboard_driver();
    log_attribute(LOG_INFO, "Loaded the keyboard driver");

    init_paging();

    enable_interrupts();
    log_attribute(LOG_INFO, "Enabled interrupts");

    kprintf("\nEnvironment information:\n");
    kprintf("Total memory: %dMB;\n", (multiboot_header_pointer->mem_upper - multiboot_header_pointer->mem_lower) / 1024);
    kprintf("Bootloader: %s;\n", (char*) multiboot_header_pointer->boot_loader_name);
    kprintf("Command line: %s;\n", (char*) multiboot_header_pointer->cmdline);

    // We need this here...
    for (;;)
        asm("nop");
}