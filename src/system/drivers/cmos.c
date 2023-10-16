#include <cmos.h>

void cmos_dump(unsigned int * values) {
	unsigned int index;
	for (index = 0; index < 128; ++index) {
		port_byte_out(0x70, index);
		values[index] = from_bcd(port_byte_in(0x71));
	}
}