#pragma once

#include <colors.h>

#define VIDEO_ADDRESS   0xB8000
#define HEIGHT          25
#define WIDTH           80
#define SCREEN_CONTROL  0x3D4
#define SCREEN_DATA     0x3D5
#define MOUSE_CURSOR    LIGHT_GREY | LIGHT_GREY << 4

struct ScreenBuffer {char text; char color;};
extern struct ScreenBuffer screen_buffer[WIDTH][HEIGHT];

void clear_screen();
void print_at(char *message, int col, int row, char color);
void print(char *message);
void println(char *message);
void print_backspace();
void color_print(char *message, char color);
void print_cursor(int prev_x, int prev_y, int mouse_x, int mouse_y);
void set_cursor(int offset);
int get_cursor();
void disable_cursor();