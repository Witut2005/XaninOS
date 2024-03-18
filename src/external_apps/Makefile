
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
	# $(C) $(C_COMPILE) -nostdlib test.c -I /home/witut/xaninOS/src/ -L /home/witut/xaninOS/src/external_apps libc.o -Ttext 0xA00000 -o test.elf
	
	# -l c 
    # rm bitmap.bmp
	# rm libc.a libc.o crt0.o 
	$(C) $(C_COMPILE) -nostdlib brainfuck.c -I ../ -L /home/witut/xaninOS/src/external_apps libc.o -Ttext 0xA00000 -o brainfuck 