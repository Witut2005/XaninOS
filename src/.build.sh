#!/bin/bash

#UBUNTU TEST
#add 32-bit gcc cross-compiler to your $PATH variable
#ASSEMBLER COMPILATION

nasm -fbin ./boot/boot.asm 
nasm -fbin ./boot/locateFS.asm
nasm -fbin ./boot/kernelLoader.asm

nasm -fbin ./xin_fs/xin_pointers.asm
nasm -fbin ./xin_fs/entries_table.asm

cd ./handlers
nasm -f elf32 keyboard.asm -o keyboard.o
cd ..

cd ./syscall
nasm -f elf32 syscall.asm -o syscall.o
cd ..

cd ./pit
nasm -f elf32 pit.asm -o pit.o
cd ..

cd ./libc
nasm -f bin enter_real_mode.asm -o enter_real_mode
cd ..

cd ./devices/HARD_DISK
nasm -f bin disk.asm -o disk
nasm -f bin load_kernel.asm -o load_kernel
cd ..
cd ..

cd ./devices/MOUSE
nasm -f elf32 mouse.asm -o mouse.o
cd ..
cd ..


cd ./xaninApps
nasm -f bin shutdown.asm -o shutdown
nasm -f bin reboot.asm -o reboot
nasm -f bin syscall_test.asm -o syscall_test
nasm -f bin xanin_apps_space.asm -o xanin_apps_space
cd ..


#COMPILE																										 HERE YOU MUST PUT PATH TO SRC DIRECTORY
#alias xanincpp='i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlibc -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src -c'

shopt -s expand_aliases
alias xanincpp='i386-elf-g++ -O0 -fno-exceptions -lstdc++ -masm=intel -std=c++17 -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -Wno-write-strings -Wno-return-type -fno-rtti -I /home/witut/Desktop/xaninOS/src -c'
alias xaninc='i386-elf-gcc -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -I /home/witut/Desktop/xaninOS/src/ -c'

xanincpp ./chip8/chip8.cpp  -o ./chip8/chip8.o
xanincpp ./devices/APIC/apic.cpp  -o ./devices/APIC/apic.o
xanincpp ./test/cpp_test.cpp  -o ./test/cpp_test.o
xanincpp ./test/cpp_test2.cpp  -o ./test/cpp_test2.o
xanincpp ./devices/IOAPIC/ioapic.cpp -o ./devices/IOAPIC/ioapic.o

#xaninc ./lib/c/c_libary.h -o ./lib/c/c_libary.o
#xaninc ./test/testc.c -o ./test/testc.o

i386-elf-gcc -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -I /home/witut/Desktop/xaninOS/src \
kernel.c \
./handlers/keyboard.o \
./pit/pit.o \
./syscall/syscall.o \
./test/cpp_test.o \
./test/cpp_test2.o \
./devices/MOUSE/mouse.o \
./devices/APIC/apic.o \
./devices/IOAPIC/ioapic.o \
./test/testc.o -o kernel.bin

cat ./xaninApps/shutdown ./elf/testc > ./xaninApps/xanin_external_apps


dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc


cat ./boot/boot ./libc/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin

#MOV XANIN.img to BIN
dd if=xanin.bin of=xanin.img
mv xanin.img -f ../bin
mv xanin.bin -f ../bin
