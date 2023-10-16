#include <keyboard.h>
#include <terminal.h>
#include <ports.h>
#include <isr.h>
#include <str.h>
#include <shell.h>

static char key_buffer[256];

const char sc_ascii[] = { '?', '?', 
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', '?', 
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '?', '?', 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'?', '`', '?', '\\', 
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t regs) {
    unsigned char scancode = port_byte_in(0x60);
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {backspace(key_buffer); print_backspace();} 
    else if (scancode == ENTER) {println(""); shell(key_buffer); key_buffer[0] = '\0';} 
    else {char letter = sc_ascii[(int)scancode]; char str[2] = {letter, '\0'}; append(key_buffer, letter); print(str);}
}

void init_keyboard() {register_interrupt_handler(IRQ+1, keyboard_callback); }