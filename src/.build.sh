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

cd ./lib
nasm -f bin enter_real_mode.asm -o enter_real_mode
cd ..

cd ./devices/HARD_DISK
nasm -f bin disk.asm -o disk
nasm -f bin load_kernel.asm -o load_kernel
cd ..
cd ..


cd ./xaninApps
nasm -f bin shutdown.asm -o shutdown
nasm -f bin reboot.asm -o reboot
nasm -f bin syscall_test.asm -o syscall_test
nasm -f bin xanin_apps_space.asm -o xanin_apps_space
cd ..

#COMPILE																										 HERE YOU MUST PUT PATH TO SRC DIRECTORY


#i386-elf-gcc -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -I /home/witut/Desktop/xaninOS/src -c ./real_mode_manager/real_mode.cpp -o ./real_mode_manager/real_mode.o


#i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
#-c ./game_engine/xagame.cpp  -o ./game_engine/xagame.o

#i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
#-c ./xagames/xagame_test.cpp  -o ./xagames/xagame_test.o

#i386-elf-g++ -std=c++2a -O0 -masm=intel -w -Wno-builtin-declaration-mismatch -Wwrite-strings -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
#-c ./assembler/assembler.cpp  -o ./assembler/assembler.o

i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
-c ./chip8/chip8.cpp  -o ./chip8/chip8.o

i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -Wno-write-strings -Wno-return-type -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
-c ./test/cpp_test.cpp  -o ./test/cpp_test.o

#i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -Wno-write-strings -Wno-return-type -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
#-c ./devices/mouse.cpp  -o ./devices/mouse.o ./devices/mouse.o


i386-elf-gcc -O0 -masm=intel -Wno-discarded-qualifiers -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -Wno-div-by-zero -I /home/witut/Desktop/xaninOS/src \
kernel.c ./handlers/keyboard.o ./pit/pit.o ./syscall/syscall.o ./test/cpp_test.o  -o kernel.bin



cat ./xaninApps/shutdown ./xaninApps/reboot ./xaninApps/syscall_test ./devices/HARD_DISK/load_kernel > ./xaninApps/xanin_external_apps

dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc

cat ./boot/boot ./lib/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin
#        0                   1                   2                   12                          16              17                                  


#MOV XANIN.img to BIN
dd if=xanin.bin of=xanin.img
mv xanin.img -f ../bin
mv xanin.bin -f ../bin
#rm xanin.bin 
#rm kernel.bin
