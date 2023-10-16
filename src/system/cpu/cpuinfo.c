#include <cpuinfo.h>
#include <terminal.h>
#include <types.h>

static inline void cpuid(unsigned int reg, unsigned int *eax, unsigned int *ebx, unsigned int *ecx, unsigned int *edx) {
    asm volatile("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) 
        : "0" (reg));
}

void cpu_vendor() {
    unsigned int func;
    char vendor[13];
    cpuid(0, &func, (unsigned int *)(vendor + 0), (unsigned int *)(vendor + 8), (unsigned int *)(vendor + 4));
    vendor[12] = '\0';
    print("CPU Vendor: ");
    println(vendor);
}

void cpu_name() {
    unsigned int eax, ebx, ecx, edx;
    unsigned int func;
    cpuid(0x80000000, &func, &ebx, &ecx, &edx);
    if (func >= 0x80000004) {
        char name[48];
        cpuid(0x80000002, (unsigned int *)(name +  0), (unsigned int *)(name +  4), (unsigned int *)(name +  8), (unsigned int *)(name + 12));
        cpuid(0x80000003, (unsigned int *)(name + 16), (unsigned int *)(name + 20), (unsigned int *)(name + 24), (unsigned int *)(name + 28));
        cpuid(0x80000004, (unsigned int *)(name + 32), (unsigned int *)(name + 36), (unsigned int *)(name + 40), (unsigned int *)(name + 44));
        const char *p = name;
        while (*p == ' ') {++p;}
        print("CPU Name: ");
        println(p);
    }
}

void cpu_x64() {
    unsigned int eax, ebx, ecx, edx;
    unsigned int func;
    cpuid(0x80000000, &func, &ebx, &ecx, &edx);
    print("Supports x64: ");
    if (func >= 0x80000001) {
        cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
        if (edx & (1 << 29)) {
            color_print("Yes\n", GREEN | BLACK << 4);
        } else {
            color_print("No\n", RED | BLACK << 4);
        }
    }
}