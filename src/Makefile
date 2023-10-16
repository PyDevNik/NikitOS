C_SOURCES = $(shell find system -name '*.c')
OBJ = ${C_SOURCES:.c=.o system/cpu/interrupt.o} 
INC = ./system/include
CC = ../i386elf/bin/i386-elf-gcc
LD = ../i386elf/bin/i386-elf-ld
NASM = nasm
CFLAGS =  -I./system/include -w -m32 -ffreestanding

../build/NikitOS.bin: system/boot/bootsect.bin kernel.bin
	cat $^ > ../build/NikitOS.bin
	${MAKE} clean

kernel.bin: system/boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c 
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	${NASM} $< -f elf -o $@

%.bin: %.asm
	${NASM} $< -f bin -o $@

clean:
	rm -rf $(shell find . -name '*.o')
	rm -rf $(shell find . -name '*.bin')