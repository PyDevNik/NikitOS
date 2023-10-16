#include <kernel.h>
#include <terminal.h>
#include <ports.h>
#include <memory.h>
#include <str.h>

int get_cursor();
void set_cursor(int offset);
int print_char(char c, int col, int row, char color);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

struct ScreenBuffer screen_buffer[WIDTH][HEIGHT];

void print_at(char *message, int col, int row, char color) {
    int offset, i = 0;
    if (col >= 0 && row >= 0) {offset = get_offset(col, row);}
    else {offset = get_cursor(); row = get_offset_row(offset); col = get_offset_col(offset);}
    while (message[i] != 0) {offset = print_char(message[i++], col, row, color); row = get_offset_row(offset); col = get_offset_col(offset);}
}

void print(char *message) {print_at(message, -1, -1, WHITE_ON_BLACK);}
void println(char *message) {print(message); print("\n");}
void print_backspace() {int offset = get_cursor()-2; print_char(0x08, get_offset_col(offset), get_offset_row(offset), WHITE_ON_BLACK);}
void color_print(char *message, char color) {print_at(message, -1, -1, color);}

void print_cursor(int prev_x, int prev_y, int mouse_x, int mouse_y) {
    int offset = get_cursor();
    char text = screen_buffer[prev_x][prev_y].text;
    char color = screen_buffer[prev_x][prev_y].color;
    screen_buffer[prev_x][prev_y].text = text;
    screen_buffer[prev_x][prev_y].color = color;
    print_char(text, prev_x, prev_y, color);
    char mouse_text = screen_buffer[mouse_x][mouse_y].text;
    char mouse_color = screen_buffer[mouse_x][mouse_y].color;
    print_char(' ', mouse_x, mouse_y, MOUSE_CURSOR);
    screen_buffer[mouse_x][mouse_y].text = mouse_text;
    screen_buffer[mouse_x][mouse_y].color = mouse_color;
    set_cursor(offset);
}

int print_char(char c, int col, int row, char color) {
    unsigned char *screen = (unsigned char*) VIDEO_ADDRESS;
    if (!color) {color = WHITE_ON_BLACK;}
    if (col >= WIDTH || row >= HEIGHT) {return get_offset(col, row);}
    int offset = get_offset(col, row);
    if (c == '\n') {print_at(" ", col, row, BLACK | BLACK << 4); row = get_offset_row(offset); offset = get_offset(0, row + 1);} 
    else if (c == 0x08) {screen[offset] = ' '; screen[offset + 1] = color;} 
    else {screen[offset] = c; screen[offset + 1] = color; offset += 2;}
    if (offset >= HEIGHT * WIDTH * 2) {
        kernel_main();
    }
    if (color != MOUSE_CURSOR) {screen_buffer[col][row].text = c; screen_buffer[col][row].color = color;}
    set_cursor(offset);
    return offset;
}


int get_cursor() {
    port_byte_out(SCREEN_CONTROL, 14);
    int offset = port_byte_in(SCREEN_DATA) << 8; 
    port_byte_out(SCREEN_CONTROL, 15);
    offset += port_byte_in(SCREEN_DATA);
    return offset * 2; 
}

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(SCREEN_CONTROL, 14);
    port_byte_out(SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(SCREEN_CONTROL, 15);
    port_byte_out(SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = WIDTH * HEIGHT;
    unsigned char *screen = (unsigned char*) VIDEO_ADDRESS;
    for (int i = 0; i < screen_size; i++) {screen[i*2] = ' '; screen[i*2+1] = WHITE_ON_BLACK;}
    screen_buffer[WIDTH][HEIGHT];
    set_cursor(get_offset(0, 0));
}

void disable_cursor() {port_byte_out(0x3D4, 10); port_byte_out(0x3D5, 32);}
int get_offset(int col, int row) {return 2 * (row * WIDTH + col);}
int get_offset_row(int offset) {return offset / (2 * WIDTH);}
int get_offset_col(int offset) {return (offset - (get_offset_row(offset)*2*WIDTH))/2;}