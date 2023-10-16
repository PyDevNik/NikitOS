#include <ipv4.h>
#include <memory.h>

ipv4_header_t ipv4_read_header(unsigned char* data)
{
    ipv4_header_t hdr = { 0 };
    memory_copy(&hdr, data, sizeof(ipv4_header_t));
    return hdr;
}