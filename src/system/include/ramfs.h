#pragma once

#include <types.h>
#include <stdbool.h>

struct File {
    char* data;
    size_t size;
};

struct File* create_file(const char* filename, size_t size);