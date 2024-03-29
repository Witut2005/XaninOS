
C	= i386-elf-gcc
CC 	= i386-elf-g++

CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wattributes -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c
PREINSTALL = true
ASM	= nasm
APPS = ./programs

HANDLERS = ./sys/interrupts/handlers

all:
	$(C) -fPIE -pie -Wno-builtin-declaration-mismatch test.c -I ../ -L /home/witut/XaninOS/src/external_apps libc.o -o test 
	@# $(C) -fPIE -pie -shared -Wno-builtin-declaration-mismatch -nostdlib brainfuck.c -I ../ -L /home/witut/XaninOS/src/external_apps libc.o -o brainfuck 