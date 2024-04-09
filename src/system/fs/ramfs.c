#include <ramfs.h>
#include <types.h>
#include <memory.h>

struct File* create_file(const char* filename, size_t size) {
    struct File* file = (struct File*)kmalloc(sizeof(struct File));
    if (file) {
        file->data = (char*)kmalloc(size);
        if (file->data) {
            file->size = size;
            return file;
        }
    }
    return NULL;
}