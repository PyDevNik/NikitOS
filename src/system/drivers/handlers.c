#include <handlers.h>
#include <mouse.h>
#include <help.h>
#include <info.h>

void process_left_click(int x, int y) {print_mouse_info();}
void process_middle_click(int x, int y) {help_program();}
void process_right_click(int x, int y) {info_program();}