#include <ethernet.h>
#include <memory.h>

ethernet_header_t ethernet_read_header(unsigned char* data)
{
    ethernet_header_t hdr = { 0 };
    memory_copy(&hdr, data, sizeof(ethernet_header_t));
    return hdr;
}