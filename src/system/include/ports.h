#pragma once

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_short_in(unsigned short port);
void port_short_out(unsigned short port, unsigned short data);
unsigned int port_long_in(unsigned short port);
void port_long_out(unsigned short port, unsigned int data);