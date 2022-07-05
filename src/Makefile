

C	= i386-elf-gcc
CC 	= i386-elf-g++

C_OPTIONS = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I /home/witut/Desktop/xaninOS/src/ -c
C_COMPILE = -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -Wno-address-of-packed-member -I /home/witut/Desktop/xaninOS/src/
CC_OPTIONS =  -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -Wno-return-type -fno-rtti -I /home/witut/Desktop/xaninOS/src -c
ASM	= nasm
APPS = ./xaninApps

HANDLERS = ./handlers

build:

	@echo [ASM] boot.asm
	@$(ASM) -fbin ./boot/boot.asm

	@echo [ASM] boot.asm
	@$(ASM) -fbin ./boot/kernelLoader.asm
	
	@echo [ASM] xin_pointers.asm
	@$(ASM) -fbin ./xin_fs/xin_pointers.asm

	@echo [ASM] entries_table.asm
	@$(ASM) -fbin ./xin_fs/entries_table.asm
	
	@echo [ASM] keyboard.asm
	@$(ASM) -f elf32 ./handlers/keyboard.asm -o ./handlers/keyboard.o

	@# echo [ASM] syscall.asm
	@# $(ASM) -f elf32 ./syscall/syscall.asm -o ./syscall/syscall.o
	

	@echo [ASM] pit.asm
	@$(ASM) -f elf32 ./pit/pit.asm -o ./pit/pit.o

	@echo [ASM] enter_real_mode.asm
	@$(ASM) -fbin ./libc/enter_real_mode.asm -o ./libc/enter_real_mode

	@echo [ASM] shutdown.asm
	@$(ASM) -fbin $(APPS)/shutdown.asm -o $(APPS)/shutdown

	@echo [ASM] reboot.asm
	@$(ASM) -fbin $(APPS)/reboot.asm -o $(APPS)/reboot

	@echo [ASM] syscall_test.asm
	@$(ASM) -fbin $(APPS)/syscall_test.asm -o  $(APPS)/syscall_test -w-other

	@echo [ASM] xanin_apps_space.asm
	@$(ASM) -fbin $(APPS)/xanin_apps_space.asm -o $(APPS)/xanin_apps_space

	@echo [CC] chip8.cpp
	@$(CC) $(CC_OPTIONS) ./chip8/chip8.cpp -o ./chip8/chip8.o
	@echo [CC] apic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/APIC/apic.cpp -o ./devices/APIC/apic.o
	@echo [CC] cpp_test.cpp
	@$(CC) $(CC_OPTIONS) ./test/cpp_test.cpp -o ./test/cpp_test.o
	@echo [CC] cpp_test.cpp
	@$(CC) $(CC_OPTIONS) ./test/cpp_test2.cpp -o ./test/cpp_test2.o
	@echo [CC] com.cpp
	@$(CC) $(CC_OPTIONS) ./devices/COM/com.cpp -o ./devices/COM/ComPort.o
	@echo [CC] ioapic.cpp
	@$(CC) $(CC_OPTIONS) ./devices/IOAPIC/ioapic.cpp -o ./devices/IOAPIC/ioapic.o
	
	@echo [CC] ne2000.cpp
	@$(CC) $(CC_OPTIONS) ./devices/NIC/ne2000.cpp -o ./devices/NIC/ne2000.o

	@echo [C] kernel.c
	@$(C) $(C_COMPILE) kernel.c \
	./handlers/keyboard.o \
	./pit/pit.o \
	./test/cpp_test.o \
	./test/cpp_test2.o \
	./devices/MOUSE/mouse.o \
	./devices/APIC/apic.o \
	./devices/NIC/ne2000.o \
	./devices/IOAPIC/ioapic.o \
	./test/testc.o \
	./devices/COM/ComPort.o -o kernel.bin

	@cat ./xaninApps/shutdown $(APPS)/current_app $(APPS)/syscall_test > ./xaninApps/xanin_external_apps
	@dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc
	@cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin

	@dd if=xanin.bin of=xanin.img
	@mv xanin.img -f ../bin
	@mv xanin.bin -f ../bin



run:
	qemu-system-i386 -net nic,model=ne2k_pci -drive file=../bin/xanin.img,cache=unsafe -m 512

curses:
	qemu-system-i386 -curses -k en-us -m 512 -hda ../bin/xanin.img 

debug:
	qemu-system-i386 -m 512 -s -S ../bin/xanin.img 

windows:
	qemu-system-i386.exe -m 512 -hda ..\bin\xanin.img
