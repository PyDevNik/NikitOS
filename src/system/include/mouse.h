#pragma once

#define MOUSE_CMD_RESET                     0xFF
#define MOUSE_CMD_RESEND                    0xFE
#define MOUSE_CMD_SET_DEFAULTS              0xF6
#define MOUSE_CMD_ENABLE_PACKET_STREAMING   0xF4
#define MOUSE_CMD_SAMPLE_RATE               0xF3
#define MOUSE_CMD_MOUSE_ID                  0xF2
#define MOUSE_CMD_REQUEST_SINGLE_PACKET     0xEB
#define MOUSE_CMD_STATUS                    0xE9
#define MOUSE_ACKNOWLEDGE                   0xFA
#define PS2_CMD_PORT                        0x64    
#define MOUSE_DATA_PORT                     0x60    

typedef struct {
    unsigned char left_button:      1;
    unsigned char right_button:     1;
    unsigned char middle_button:    1;
} MouseStatus;

void mouse_init();
void print_mouse_info();
int mouse_getx();
int mouse_gety();