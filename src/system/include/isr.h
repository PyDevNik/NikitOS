#pragma once

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

typedef struct {
   unsigned int ds; 
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; 
   unsigned int int_no, err_code; 
   unsigned int eip, cs, eflags, useresp, ss; 
} registers_t;

#define IRQ 0x20
#define low_16(address)    (unsigned short)((address) & 0xFFFF)
#define high_16(address)   (unsigned short)(((address) >> 16) & 0xFFFF)

void isr_install();
void irq_install();
void isr_handler(registers_t r);

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(unsigned char n, isr_t handler);