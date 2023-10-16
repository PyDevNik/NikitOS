#include <kernel.h>
#include <shell.h>
#include <terminal.h>
#include <mouse.h>
#include <str.h>
#include <info.h>
#include <help.h>
#include <time.h>
#include <cpu.h>

char *name = "$";

void shell(char *input) {
    char * command; char * args;
    split_input(input, command, args);
    if (strcmp(command, "INFO") == 0) {
        info_program();
    } else if (strcmp(command, "HELP") == 0) {
        help_program();
    } else if (strcmp(command, "CLEAR") == 0) {
        kernel_main();
    } else if (strcmp(command, "CPU") == 0) {
        cpu_program();
    } else if (strcmp(command, "TIME") == 0) {
        time_program();
    } else if (strcmp(command, "END") == 0) {
        clear_screen();
        asm volatile("hlt");
    } else if (strcmp(command, "MOUSE") == 0) {
        print_mouse_info();
    } else if (strcmp(command, "LOGIN") == 0) {
        if (args != "") {
            print("Logged in as ");
            print(args);
            name = args;
        }
    } else if (strcmp(command, "WHOAMI") == 0) {
        print(name);
    } else {
        print("Unknown Option: ");
        print(input);
        }
    print("\n");
    print(name);
    color_print(" >>> ", GREEN | BLACK << 4);
}

void split_input(char *input, char *command, char *argument) {
    int i = 0;
    while (input[i] != ' ' && input[i] != '\0') {command[i] = input[i]; i++;}
    command[i] = '\0';
    while (input[i] == ' ') {i++;}
    int j = 0;
    while (input[i] != '\0') {argument[j] = input[i]; i++; j++;}
    argument[j] = '\0';
}