#include <net.h>
  
unsigned short swap_uint16(unsigned short val)
{
    return (val << 8) | (val >> 8 );
}
  
short swap_int16(short val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

unsigned int swap_uint32(unsigned int val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}
 
int swap_int32(int val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}