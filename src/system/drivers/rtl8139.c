#include <rtl8139.h>
#include <terminal.h>
#include <str.h>
#include <memory.h>
#include <ports.h>
#include <pci.h>
#include <isr.h>
#include <types.h>
#include <ports.h>
#include <net.h>
#include <ipv4.h>
#include <ethernet.h>

static pci_device_t device[2] = {0};
static unsigned int ioaddr[2];
static unsigned char* tx_buffer[2];
static unsigned char* rx_buffer[2];
static unsigned int rx_index[2] = {0};
static unsigned char tx_counter[2] = {0};
static unsigned char mac_addr[2][6];

isr_t handler_picker(unsigned char device_index);
static void rtl8139_handler(registers_t* regs, unsigned char device_index);
static void rtl8139_handler0(registers_t* regs);
static void rtl8139_handler1(registers_t* regs);
static void rtl8139_wrong_handler(registers_t* regs);
static void rtl8139_receive_packet(unsigned char device_index);
void rtl8139_init_device(int device_index);

void rtl8139_init()
{
    rtl8139_init_device(0);
    rtl8139_init_device(1);
}

void rtl8139_init_device(int device_index)
{    
    print("[RTL8139] Initializing #");
    char test_buffer[2];
    println(int_to_ascii(device_index, test_buffer, 10));
    device[device_index] = pciGetDevice(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, device_index);
    if (device[device_index].address == 0) {println("[RTL8139] Initialization failed\n"); return FALSE;}
    ioaddr[device_index] = device[device_index].bar0 & 0xFFFFFFFC;
    unsigned int pci_command_register = pciRead(device[device_index].address, PCI_COMMAND);
    if (!(pci_command_register & (1 << 2))) {pci_command_register |= (1 << 2); pciWrite(device[device_index].address, PCI_COMMAND, pci_command_register);}
    port_byte_out(ioaddr[device_index] + 0x52, 0x0);                                     
    port_byte_out(ioaddr[device_index] + 0x37, 0x10); 
    while ( (port_byte_in(ioaddr[device_index] + 0x37) & 0x10) != 0) {}
    rx_buffer[device_index] = (unsigned char*)kmalloc(RTL8139_RX_SIZE, 1);
    port_long_out(ioaddr[device_index] + 0x30, (unsigned long long)rx_buffer[device_index]);
    memory_set(rx_buffer[device_index], 0, RTL8139_RX_SIZE);
    tx_buffer[device_index] = (unsigned char*)kmalloc(RTL8139_TX_SIZE, 0);
    port_word_out(ioaddr[device_index] + 0x3C, 0x0005);                                  
    port_long_out(ioaddr[device_index] + 0x44, 0xf | (1 << 7));
    port_byte_out(ioaddr[device_index] + 0x37, 0x0C);                                    
    unsigned char irq_num = (unsigned char)(pciRead(device[device_index].address, PCI_INTERRUPT_LINE) & 0xFF);
    isr_t handler = handler_picker(device_index);
    if (handler == rtl8139_wrong_handler) {println("[RTL8139] Wrong handler chosen\n"); return FALSE;}
    register_interrupt_handler(IRQ + irq_num, handler);
    print("[RTL8139] Registered at #");
    char irq_number_str[4];
    println(int_to_ascii(irq_num+IRQ, irq_number_str, 16));
    for (unsigned long long i = 0; i < 6; ++i)
    {mac_addr[device_index][i] = port_byte_in(ioaddr[device_index] + RTL8139_MAC_OFFSET + i);}
    char mac_byte_hex[3];
    println("[RTL8139] MAC address is: ");
    print(int_to_ascii(mac_addr[device_index][0], mac_byte_hex, 16));
    print(":"); 
    print(int_to_ascii(mac_addr[device_index][1], mac_byte_hex, 16));
    print(":");
    print(int_to_ascii(mac_addr[device_index][2], mac_byte_hex, 16));
    print(":");
    print(int_to_ascii(mac_addr[device_index][3], mac_byte_hex, 16));
    print(":");
    print(int_to_ascii(mac_addr[device_index][4], mac_byte_hex, 16));
    print(":");
    print(int_to_ascii(mac_addr[device_index][5], mac_byte_hex, 16));
    println("\n");
}

isr_t handler_picker(unsigned char device_index)
{
    switch (device_index)
    {
    case 0:
        return rtl8139_handler0;
    case 1:
        return rtl8139_handler1;
    default:
        return rtl8139_wrong_handler;
    }
}

static void rtl8139_wrong_handler(registers_t* regs) {}
static void rtl8139_handler0(registers_t* regs) {rtl8139_handler(regs, 0);}
static void rtl8139_handler1(registers_t* regs) {rtl8139_handler(regs, 1);}

static void rtl8139_handler(registers_t* regs, unsigned char device_index)
{
    while (TRUE)
    {
        unsigned short status = port_word_in(ioaddr[device_index] + RTL8139_ISR_OFFSET);
        port_word_out(ioaddr[device_index] + RTL8139_ISR_OFFSET, status); 
        if (status == 0) {break;}
        if (status & RTL8139_TOK) {}
        if (status & RTL8139_ROK) {rtl8139_receive_packet(device_index);}
    }  
}

void rtl8139_send_packet(void* packet, unsigned int size, unsigned char device_index)
{
    unsigned char cnt = tx_counter[device_index];
    memory_copy(tx_buffer[device_index], packet, size);
    port_long_out(ioaddr[device_index] + 0x20 + cnt * 4, (unsigned int)tx_buffer[device_index]);
    port_long_out(ioaddr[device_index] + 0x10 + cnt * 4, size);
    tx_counter[device_index] = (cnt >= 3) ? 0 : cnt + 1;
    println("Transmitted a packet - device #");
    print(device_index + '0');
    print('\n');
}

static void rtl8139_receive_packet(unsigned char device_index)
{
    println("Landed receive");
    unsigned int index = rx_index[device_index];
    while ((port_byte_in(ioaddr[device_index] + RTL8139_COMMAND) & 0x1) == 0)  
    {
        unsigned int offset = index % RTL8139_RX_SIZE;
        unsigned int read_ptr = (unsigned int)rx_buffer[device_index] + offset;
        rtl8139_header* rx_header_ptr =  (rtl8139_header*)read_ptr;
        if (rx_header_ptr->status & (
                                    RTL8139_RX_PACKET_HEADER_FAE  | 
                                    RTL8139_RX_PACKET_HEADER_CRC  | 
                                    RTL8139_RX_PACKET_HEADER_RUNT | 
                                    RTL8139_RX_PACKET_HEADER_LONG   
                                    )
            )
        {println("[RTL8139] ERR: rx packet header error\n");}
        else
        {
            unsigned char transmit_buf[RTL8139_TX_SIZE];
            unsigned char* buf = (unsigned char*)(read_ptr + sizeof(rtl8139_header));
            ethernet_header_t eth_header = ethernet_read_header(buf);
            buf += sizeof(ethernet_header_t);
            print("Checking ethernet type - device #");
            print(device_index + '0');
            print('\n');
            println("Type - ");
            int n[10];
            println(int_to_ascii(swap_uint16(eth_header.type), n, 16));
            print('\n');
            if (swap_uint16(eth_header.type) == ETHERTYPE_IPV4)
            {
                println("[Ethernet] Recieved a packet to transfer\n");
                ipv4_header_t ipv4_header = ipv4_read_header(buf);
                buf += sizeof(ipv4_header); 
                if (ipv4_header.version == 4 && ipv4_header.ttl != 0)
                {
                    println("[IPv4] Recieved a packet to transfer\n");
                    unsigned char other_NIC = device_index ^ 1;
                    memory_copy(&eth_header.src, mac_addr[other_NIC], 6); 
                    memory_set(&eth_header.dst, 0xFF, 6); 
                    ipv4_header.ttl -= 0x1;
                    unsigned char* tbuf_ptr = transmit_buf;
                    memory_copy(tbuf_ptr, &eth_header, sizeof(ethernet_header_t));
                    tbuf_ptr += sizeof(ethernet_header_t);
                    memory_copy(tbuf_ptr, &ipv4_header, sizeof(ipv4_header_t));
                    tbuf_ptr += sizeof(ipv4_header_t);
                    unsigned short total_len = swap_uint16(ipv4_header.len);
                    unsigned char ipv4_left_to_read = total_len - 0x20;   
                    memory_copy(tbuf_ptr, buf, ipv4_left_to_read);       
                    rtl8139_send_packet(transmit_buf, 0x14 + total_len, other_NIC);
                }
            }
        }
        index = (index + rx_header_ptr->size + sizeof(rtl8139_header) + 3) & 0xFFFFFFFC;
        port_word_out(ioaddr[device_index] + RTL8139_RX_BUF_PTR, index - 0x10);
    }
    rx_index[device_index] = index;
}