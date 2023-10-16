#include <terminal.h>
#include <isr.h>
#include <ports.h>
#include <str.h>
#include <pci.h>
#include <registers.h>
#include <types.h>
#include <stdbool.h>

bool start = FALSE;

void kernel_main() {
    clear_screen();
    if (!start) {disable_cursor(); isr_install(); irq_install(); start = TRUE;}
    color_print("\nWelcome!\n", LIGHT_CYAN | BLACK << 4);
    color_print("\nNikitOS\n\n", RED_ON_WHITE);
    color_print("$", CYAN_ON_BLACK);
    color_print(" >>> ", GREEN | BLACK << 4);
    println("No exit!!!NO!!");
}