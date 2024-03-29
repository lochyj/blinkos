#include "cpu/irq.h"

__attribute((aligned(0x10)))
isr_t interrupt_handlers[256];

__attribute((aligned(0x10)))
static idt_entry_t idt[256];

static idt_ptr_t idtr;

char* exceptions_0_to_16[] = {
    "Divide error",                             // 0
    "Debug exception",                          // 1
    "Nonmaskable interrupt",                    // 2
    "Breakpoint (one-byte INT 3 instruction)",  // 3
    "Overflow (INTO instruction)",              // 4
    "Bounds check (BOUND instruction)",         // 5
    "Invalid opcode",                           // 6
    "Coprocessor not available",                // 7
    "Double fault",                             // 8
    "(reserved)",                               // 9
    "Invalid TSS",                              // 10
    "Segment not present",                      // 11
    "Stack exception",                          // 12
    "General protection",                       // 13
    "Page fault",                               // 14
    "(reserved)",                               // 15
    "Coprocessor error",                        // 16
};

/* Wikipedia page on IRQ's:
Master PIC

    IRQ 0 – system timer (cannot be changed)
    IRQ 1 – keyboard on PS/2 port (cannot be changed)
    IRQ 2 – cascaded signals from IRQs 8–15 (any devices configured to use IRQ 2 will actually be using IRQ 9)
    IRQ 3 – serial port controller for serial port 2 (shared with serial port 4, if present)
    IRQ 4 – serial port controller for serial port 1 (shared with serial port 3, if present)
    IRQ 5 – parallel port 3 or sound card
    IRQ 6 – floppy disk controller
    IRQ 7 – parallel port 1 (shared with parallel port 2, if present). It is used for printers or for any parallel port if a printer is not present. It can also be potentially be shared with a secondary sound card with careful management of the port.

Slave PIC

    IRQ 8 – real-time clock (RTC)
    IRQ 9 – Advanced Configuration and Power Interface (ACPI) system control interrupt on Intel chipsets.[3] Other chipset manufacturers might use another interrupt for this purpose, or make it available for the use of peripherals (any devices configured to use IRQ 2 will actually be using IRQ 9)
    IRQ 10 – The Interrupt is left open for the use of peripherals (open interrupt/available, SCSI or NIC)
    IRQ 11 – The Interrupt is left open for the use of peripherals (open interrupt/available, SCSI or NIC)
    IRQ 12 – mouse on PS/2 port
    IRQ 13 – CPU co-processor or integrated floating point unit or inter-processor interrupt (use depends on OS)
    IRQ 14 – primary ATA channel (ATA interface usually serves hard disk drives and CD drives)
    IRQ 15 – secondary ATA channel
*/

void register_interrupt_handler(uint8_t vector, isr_t handler) {
    IRQ_clear_mask(vector);
    interrupt_handlers[vector] = handler;
}

void set_idt_gate(uint8_t gate_index, uint32_t base, uint16_t selector, uint8_t gate_type_attributes) {
    // 16 bits is equal to 0xFFFF. We firstly remove the first 16 bits that we used for base_low and then mask the last 16 bits for the base_high.
    idt[gate_index].base_low = base & 0xFFFF;
    idt[gate_index].base_high = (base >> 16) & 0xFFFF;
    idt[gate_index].selector = selector;
    idt[gate_index].zero = 0;   // Zero should ALWAYS be 0 no matter what. This is reserved for intel to do what they like with.
    idt[gate_index].gate_type_attributes = gate_type_attributes;

}

void isr_handler(registers_t* regs) {

    if (interrupt_handlers[regs->int_no] != NULL) {

        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);

    } else {

        if (regs->int_no < 17) {
            kprintf("Unhandled interrupt: %s\n", exceptions_0_to_16[regs->int_no]);
        } else if (regs->int_no < 32) {
            kprintf("Unhandled reserved interrupt\n");
        } else {
            kprintf("Unhandled external interrupt: ISR %d\n", regs->int_no);
        }

    }

    return;

}

void irq_handler(registers_t* regs) {

    // Debugging...
    //kprintf("IRQ: %d\r", regs->int_no);

    // Call the function at the index of the interrupt number
    if (interrupt_handlers[regs->int_no] != NULL) {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    // Signal to the PIC that the interrupt is finished...

    if (regs->int_no >= IRQ8) { // If the interrupt came from the slave PIC
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);

    return;

}

void initialise_idt() {
    idtr.limit = sizeof(idt_entry_t) * 256 - 1;
    idtr.base = (uint32_t) &idt;

    memset(&idt, 0, sizeof(idt_entry_t) * 256);

    // Remapping the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Set our IDT gates
    set_idt_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    set_idt_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    set_idt_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    set_idt_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    set_idt_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    set_idt_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    set_idt_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    set_idt_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    set_idt_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    set_idt_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    set_idt_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    set_idt_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    set_idt_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    set_idt_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    set_idt_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    set_idt_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    set_idt_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    set_idt_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    set_idt_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    set_idt_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    set_idt_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    set_idt_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    set_idt_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    set_idt_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    set_idt_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    set_idt_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    set_idt_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    set_idt_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    set_idt_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    set_idt_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    set_idt_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    set_idt_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    set_idt_gate(32, (uint32_t)irq0 , 0x08, 0x8E);
    set_idt_gate(33, (uint32_t)irq1 , 0x08, 0x8E);
    set_idt_gate(34, (uint32_t)irq2 , 0x08, 0x8E);
    set_idt_gate(35, (uint32_t)irq3 , 0x08, 0x8E);
    set_idt_gate(36, (uint32_t)irq4 , 0x08, 0x8E);
    set_idt_gate(37, (uint32_t)irq5 , 0x08, 0x8E);
    set_idt_gate(38, (uint32_t)irq6 , 0x08, 0x8E);
    set_idt_gate(39, (uint32_t)irq7 , 0x08, 0x8E);
    set_idt_gate(40, (uint32_t)irq8 , 0x08, 0x8E);
    set_idt_gate(41, (uint32_t)irq9 , 0x08, 0x8E);
    set_idt_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    set_idt_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    set_idt_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    set_idt_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    set_idt_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    set_idt_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    for (int i = 0; i < 256; i ++) {
        interrupt_handlers[i] = NULL;
    }

    load_idt((uint32_t)&idtr);

}

void IRQ_set_all_mask() {
    for (int8_t i = 0; i < 16; i++){
        IRQ_set_mask(i);
    }
}

void IRQ_clear_all_mask() {
    for (int8_t i = 0; i < 16; i++){
        IRQ_clear_mask(i);
    }
}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}
