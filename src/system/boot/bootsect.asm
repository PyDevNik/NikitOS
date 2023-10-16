[org 0x7c00]
KERNEL_OFFSET equ 0x1000 
BOOT_DRIVE db 0x80 

mov [BOOT_DRIVE], dl 
mov bp, 0x9000
mov sp, bp
call load_kernel 
call switch_to_pm 
jmp $ 

%include "system/boot/print_hex.asm"
%include "system/boot/disk.asm"
%include "system/boot/gdt.asm"
%include "system/boot/print16.asm"
%include "system/boot/switch.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET 
    mov dh, 31 
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET 
    jmp $ 

times 510 - ($-$$) db 0
dw 0xaa55