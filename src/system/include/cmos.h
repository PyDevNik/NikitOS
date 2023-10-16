#pragma once

#define from_bcd(val) (val & 0xF) + ((val >> 4) * 10)
void cmos_dump();