#pragma once

#include <stddef.h>
unsigned int kmalloc(unsigned int size, int align);
void *memory_set(void *dest, int value, size_t num_bytes);
void *memory_copy(void *dest, const void *src, size_t num_bytes);