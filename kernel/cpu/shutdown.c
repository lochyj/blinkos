#include "cpu/power.h"

// Info on this: https://wiki.osdev.org/Shutdown

// We have other shutdown options. This may change but it just makes it simpler for now.

void shutdown(int hardware) {

    // We don't want any interrupts anymore
    // So stop them. (Ofc we cant stop NMIs)
    asm volatile("cli");

    switch (hardware) {
        case ACPI:
            log_attribute(LOG_FATAL, "ACPI shutdown is not yet supported");
            break;
        case BOCHS:
            shutdown_bochs();
            break;
        case QEMU:
            shutdown_qemu();
            break;
        case VIRTUALBOX:
            shutdown_virtualbox();
            break;
        default:
            // Something went wrong...
            break;
    }
}

void shutdown_acpi() {
    // TODO
    // This may take alot of work...
}

// This also works for older versions of QEMU
void shutdown_bochs() {
    outw(0xB004, 0x2000);
}

void shutdown_qemu() {
    outw(0x604, 0x2000);
}

void shutdown_virtualbox() {
    outw(0x4004, 0x3400);
}
