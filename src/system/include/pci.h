#pragma once

#define PCI_CONFIG_ADDR_PORT    0xCF8
#define PCI_CONFIG_DATA_PORT    0xCFC
#define PCI_NON_EXISTENT        0xFFFF
#define PCI_ENABLE_BIT          0x80000000
#define PCI_BAR0                0x10
#define PCI_COMMAND             0x4
#define PCI_INTERRUPT_LINE      0x3C

typedef struct
{
    unsigned int address;
    unsigned int device_id, vendor_id;
    unsigned int bar0, bar1, bar2, bar3, bar4, bar5;
} pci_device_t;


unsigned int pciRead(unsigned int address, unsigned char offset);
void pciWrite(unsigned int address, unsigned char offset, unsigned int data);
pci_device_t pciGetDevice(unsigned short vendor_id, unsigned short device_id, unsigned char index);