#include <ports.h>

unsigned char port_byte_in(unsigned short port)
{
    unsigned char result = 0;
    asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned short port_word_in(unsigned short port)
{
    unsigned short result = 0;
    asm volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;

}

void port_word_out(unsigned short port, unsigned short data)
{
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

unsigned int port_long_in(unsigned short port)
{
    unsigned int result = 0;
    asm volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;

}

void port_long_out(unsigned short port, unsigned int data)
{
    asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}