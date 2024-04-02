
C	= i386-elf-gcc-12.2.0
CC 	= i386-elf-g++

CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -fno-rtti -fconcepts-ts -I $(XANIN_PATH) -c
C_OPTIONS  =  -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wattributes -Wno-unused-function -Werror=return-type -I $(XANIN_PATH) -c
PREINSTALL = true
ASM	= nasm
APPS = ./programs

HANDLERS = ./sys/interrupts/handlers

all:
	# $(C) -O0 -fpic -shared shared.c -o libshared.so
	# $(C) -O0 -g -nostdlib -fPIE -pie test.c -o test -L. -lshared
	@#$(C) shared.c -g -O0 -fpic -shared -c -o shared.so
	@#$(C) -O0 -g -fPIE -pie -nostdlib shared.so -shared -Wno-builtin-declaration-mismatch test.c -I ../ -L /home/witut/XaninOS/src/external_apps libc.a -o test 

pie:
	$(C) -Wno-builtin-declaration-mismatch -fPIE -pie -nostdlib brainfuck.c -I ../ -L /home/witut/XaninOS/src/external_apps libc.a -o brainfuck 

exec:
	$(C) -Wno-builtin-declaration-mismatch -nostdlib brainfuck.c -I ../ -L /home/witut/XaninOS/src/external_apps libc.a -o brainfuck 