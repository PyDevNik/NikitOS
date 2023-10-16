#include <time.h>
#include <cmos.h>
#include <terminal.h>
#include <str.h>

unsigned int values[128];
char *buffer;

void print_time(char *buf, int reg) {
    print(int_to_ascii(values[reg], buf, 10));
}

void time_program() {
	cmos_dump(values);
    print("Hour: ");
    print_time(buffer, 4);
    println("");
    print("Minute: ");
    print_time(buffer, 2);
    println("");
    print("Day: ");
    print_time(buffer, 7);
    println("");
    print("Month: ");
    print_time(buffer, 8);
    println("");
    print("Year: ");
    print_time(buffer, 32);
    print_time(buffer, 9);
    println("\n");
    }
