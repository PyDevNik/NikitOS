#include <memory.h>
#include <types.h>
#include <terminal.h>

unsigned char* memory_pool = (unsigned char*)0x100000; 
unsigned int memory_pool_index = 0;

unsigned int kmalloc(unsigned int size, int align) {
    if (align && (memory_pool_index & 0xFFFFF000)) {memory_pool_index &= 0xFFFFF000; memory_pool_index += 0x1000;}    
    unsigned int ptr = (unsigned int)&memory_pool[memory_pool_index];
    memory_pool_index += size;
    return ptr;
}

void *memory_set(void *dest, int value, size_t num_bytes) {
    unsigned char *ptr = (unsigned char *)dest;
    for (size_t i = 0; i < num_bytes; i++) {*ptr++ = (unsigned char)value;}
    return dest;
    }

void *memory_copy(void *dest, const void *src, size_t num_bytes) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < num_bytes; i++) {*d++ = *s++;}
    return dest;
}
