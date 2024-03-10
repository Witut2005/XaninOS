
C	= i386-elf-gcc
CC 	= i386-elf-g++

XANIN_PATH = /home/witut/xaninOS/src/
XANIN_BINARIES = /home/witut/Desktop/xaninOS/bin

CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wattributes -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c
PREINSTALL = true
ASM	= nasm
APPS = ./programs

HANDLERS = ./sys/interrupts/handlers

all:
	nasm -f elf32 write.asm -o write.o
	i386-elf-ld write.o -o write 

	nasm -f elf32 read.asm -o read.o
	i386-elf-ld read.o -o read 

	nasm -fbin reboot.asm -o reboot
	# i386-elf-ld reboot.o -o reboot 

	nasm -f elf32 link_create.asm -o link_create.o
	i386-elf-ld link_create.o -o link_create 

	$(C) $(C_COMPILE) -nostdlib brainfuck.c -I /home/witut/xaninOS/src/ -L /home/witut/xaninOS/src/external_apps -o brainfuck -l c 

    # rm bitmap.bmp
	rm libc.a libc.o crt0.o 