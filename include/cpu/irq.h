#pragma once

#include <stdint.h>
#include <string.h>

#include "logging.h"
#include "asm/i386/ports.h"

typedef struct {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;   // This is always 0
    uint8_t gate_type_attributes;   // Is it a trap type or an interrupt gate. Or a task gate but we wont be using it for that atm.
    uint16_t base_high;
} idt_entry_t;

// Derived from figure. 9-2 of the 80386 Programmer's Reference
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;    // This needs to be packed

typedef struct {
    uint32_t ds;
    uint32_t edi, esi;
    uint32_t ebp, esp;
    uint32_t ebx, edx;
    uint32_t ecx, eax;
    uint32_t int_no, err_code;              // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;  // This is pushed by the processor automatically.
} registers_t;

typedef void (*isr_t)(registers_t *);

extern void load_idt(uint32_t);

extern void enable_interrupts();
extern void disable_interrupts();

void initialise_idt();

void irq_handler(registers_t* regs);
void isr_handler(registers_t* regs);

void set_idt_gate(uint8_t gate_index, uint32_t base, uint16_t selector, uint8_t gate_type_attributes);

void register_interrupt_handler(uint8_t vector, isr_t handler);

void IRQ_clear_all_mask();
void IRQ_clear_mask(uint8_t IRQline);

// This is a little ugly but its necessary
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
