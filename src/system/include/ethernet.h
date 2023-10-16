#pragma once

#define ETHERTYPE_IPV4  0x0800
#define ARP             0x0806

typedef struct
{
    unsigned char   dst[6];
    unsigned char   src[6];
    unsigned short  type;  
} __attribute__((packed)) ethernet_header_t;


ethernet_header_t ethernet_read_header(unsigned char* data);