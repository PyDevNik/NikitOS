#pragma once

#include <types.h>

#define RTL8139_VENDOR_ID               0x10ec
#define RTL8139_DEVICE_ID               0x8139
#define RTL8139_RX_SIZE                 1536
#define RTL8139_TX_SIZE                 1536
#define RTL8139_MAC_OFFSET              0
#define RTL8139_ISR_OFFSET              0x3E
#define RTL8139_TOK                     (1 << 2)
#define RTL8139_ROK                     (1 << 0)
#define RTL8139_COMMAND                 0x37
#define RTL8139_RX_BUF_PTR              0x38
#define RTL8139_RX_PACKET_HEADER_FAE    0x2
#define RTL8139_RX_PACKET_HEADER_CRC    0x4
#define RTL8139_RX_PACKET_HEADER_RUNT   0x10
#define RTL8139_RX_PACKET_HEADER_LONG   0x8

typedef struct
{
    unsigned short status;
    unsigned short size;
} __attribute__((packed)) rtl8139_header;


void rtl8139_init();
void rtl8139_send_packet(void* packet, unsigned int size, unsigned char device_index);