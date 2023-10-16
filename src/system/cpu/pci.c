#include <pci.h>
#include <ports.h>

static unsigned int pciGetAddress(unsigned char bus, unsigned char slot, unsigned char func)
{
    unsigned int lbus  = (unsigned int)bus;
    unsigned int lslot = (unsigned int)slot;
    unsigned int lfunc = (unsigned int)func;
    return (lbus << 16) | (lslot << 11) | (lfunc << 8) | PCI_ENABLE_BIT;   
}
 
unsigned int pciRead(unsigned int address, unsigned char offset)
{
    port_long_out(PCI_CONFIG_ADDR_PORT, address | (offset & 0xFC));
    return port_long_in(PCI_CONFIG_DATA_PORT);
}

void pciWrite(unsigned int address, unsigned char offset, unsigned int data)
{
    port_long_out(PCI_CONFIG_ADDR_PORT, address | (offset & 0xFC));
    port_long_out(PCI_CONFIG_DATA_PORT, data);
}

unsigned short pciGetVendorId(unsigned int address)
{
    unsigned int vendor_id = pciRead(address, 0) & 0xFFFF;
    return vendor_id;
}

unsigned short pciGetDeviceId(unsigned int address)
{
    unsigned int device_id = (pciRead(address, 0) >> 16) & 0xFFFF;
    return device_id;
}
 
pci_device_t pciGetDevice(unsigned short vendor_id, unsigned short device_id, unsigned char index)
{
    for (int bus = 0; bus < 256; ++bus)
    {
        for (unsigned char slot = 0; slot < 32; ++slot)
        {
            for (unsigned char func = 0; func < 1; ++func)
            {
                unsigned int address = pciGetAddress(bus, slot, func);
                unsigned short v_id = pciGetVendorId(address);
                if (v_id == PCI_NON_EXISTENT) {continue;}
                unsigned short d_id = pciGetDeviceId(address);
                if (vendor_id == v_id && device_id == d_id)
                {
                    if (index == 0)
                    {
                        pci_device_t device = { 0 };
                        device.address = address;
                        device.vendor_id = vendor_id;
                        device.device_id = device_id;
                        device.bar0 = pciRead(address, PCI_BAR0);
                        return device;
                    }
                    --index;
                }
            }
        }
    }
    return (pci_device_t){ 0 };
}