#pragma once

typedef struct {
    unsigned int protectionEnable : 1;
} cr0_register;

unsigned int get_eax();
unsigned int get_ebx();
unsigned int get_ecx();
unsigned int get_edx();
unsigned int get_cr0();