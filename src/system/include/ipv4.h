#pragma once

typedef struct
{
    unsigned char   ihl:                4;     
    unsigned char   version:            4; 
    unsigned char   service_type;
    unsigned short  len;
    unsigned short  identification;
    unsigned short  fragment_offset:    13;   
    unsigned char   flags:              3;               
    unsigned char   ttl;
    unsigned char   protocol;
    unsigned short  checksum;
    unsigned int    src;
    unsigned int    dst;
} __attribute__((packed)) ipv4_header_t;

ipv4_header_t ipv4_read_header(unsigned char* data);