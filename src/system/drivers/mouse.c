#include <mouse.h>
#include <handlers.h>
#include <terminal.h>
#include <isr.h>
#include <types.h>
#include <memory.h>
#include <str.h>
#include <stdbool.h>

char * text_buffer;
int prev_x = 0, prev_y = 0, mouse_x = 0, mouse_y = 0;
unsigned int under_mouse_buffer = NULL;
MouseStatus g_status;

int mouse_getx() {return mouse_x;}
int mouse_gety() {return mouse_y;}

void mouse_wait(bool type) {
    unsigned int time_out = 100000;
    if (type == FALSE) {while (time_out--) {if ((port_byte_in(PS2_CMD_PORT) & 1) == 1) {return;}}; return;} 
    else {while (time_out--) {if ((port_byte_in(PS2_CMD_PORT) & 2) == 0) {return;}}
    }
}

void mouse_write(unsigned char data) {
    mouse_wait(TRUE);
    port_byte_out(PS2_CMD_PORT, 0xD4);
    mouse_wait(TRUE);
    port_byte_out(MOUSE_DATA_PORT, data);
}

unsigned char mouse_read() {mouse_wait(FALSE); return port_byte_in(MOUSE_DATA_PORT);}

void get_MouseStatus(char status_byte, MouseStatus *status) {
    memory_set(status, 0, sizeof(MouseStatus));
    if (status_byte & 0x01) status->left_button = 1;
    if (status_byte & 0x02) status->right_button = 1;
    if (status_byte & 0x04) status->middle_button = 1;
}

void print_mouse_info() {
    print("X: ");
    println(int_to_ascii(mouse_x, text_buffer, 10));
    print("Y: ");
    println(int_to_ascii(mouse_y, text_buffer, 10));
    if (g_status.left_button) {println("Left button clicked");}
    if (g_status.right_button) {println("Right button clicked");}
    if (g_status.middle_button) {println("Middle button clicked");}
}

void mouse_handler(registers_t *r) {
    static unsigned char mouse_cycle = 0;
    static char mouse_byte[3];
    switch (mouse_cycle) {
        case 0:
            mouse_byte[0] = mouse_read();
            get_MouseStatus(mouse_byte[0], &g_status);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = mouse_read();
            mouse_x = mouse_x + mouse_byte[1];
            mouse_y = mouse_y - mouse_byte[2];
            if (mouse_x < 0) {mouse_x = 0;}
            if (mouse_y < 0) {mouse_y = 0;}
            if (mouse_x > WIDTH) {mouse_x = WIDTH-2;}
            if (mouse_y > HEIGHT) {mouse_y = HEIGHT-2;}
            if (g_status.left_button) {process_left_click(mouse_x, mouse_y);}
            if (g_status.middle_button) {process_middle_click(mouse_x, mouse_y);}
            if (g_status.right_button) {process_right_click(mouse_x, mouse_y);}
            print_cursor(prev_x, prev_y, mouse_x, mouse_y);
            prev_x = mouse_x;
            prev_y = mouse_y;
            mouse_cycle = 0;
            break;
    }
}

void set_mouse_rate(unsigned char rate) {
    unsigned char status;
    port_byte_out(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {return;}
    port_byte_out(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {return;}
}

void mouse_init() {
    unsigned char status;
    mouse_wait(TRUE);
    port_byte_out(PS2_CMD_PORT, 0xA8);
    port_byte_out(MOUSE_DATA_PORT, MOUSE_CMD_MOUSE_ID);
    status = mouse_read();
    set_mouse_rate(10);
    mouse_wait(TRUE);
    port_byte_out(PS2_CMD_PORT, 0x20);
    mouse_wait(FALSE);
    status = (port_byte_in(MOUSE_DATA_PORT) | 2);
    mouse_wait(TRUE);
    port_byte_out(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(TRUE);
    port_byte_out(MOUSE_DATA_PORT, status);
    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {return;}
    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {return;}
    register_interrupt_handler(0x20 + 12, mouse_handler);
}