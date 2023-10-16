#include <registers.h>

unsigned int get_eax() {
    unsigned int value;
    asm volatile("movl %%eax, %0" : "=r"(value));
    return value;
}

unsigned int get_ebx() {
    unsigned int value;
    asm volatile("movl %%ebx, %0" : "=r"(value));
    return value;
}

unsigned int get_ecx() {
    unsigned int value;
    asm volatile("movl %%ecx, %0" : "=r"(value));
    return value;
}

unsigned int get_edx() {
    unsigned int value;
    asm volatile("movl %%edx, %0" : "=r"(value));
    return value;
}

unsigned int get_cr0() {
    unsigned int cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    return cr0;
}
