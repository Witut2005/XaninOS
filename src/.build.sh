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

cd ./xaninApps
nasm -f bin shutdown.asm -o shutdown
nasm -f bin syscall_test.asm -o syscall_test
nasm -f bin xanin_apps_space.asm -o xanin_apps_space
cd ..

#COMPILE																										 HERE YOU MUST PUT PATH TO SRC DIRECTORY


#i386-elf-gcc -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -I /home/witut/Desktop/xaninOS/src -c ./real_mode_manager/real_mode.cpp -o ./real_mode_manager/real_mode.o


#i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
#-c ./game_engine/xagame.cpp  -o ./game_engine/xagame.o

i386-elf-g++ -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function -fno-rtti -I /home/witut/Desktop/xaninOS/src \
-c ./xagames/xagame_test.cpp  -o ./xagames/xagame_test.o


i386-elf-gcc -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -I /home/witut/Desktop/xaninOS/src \
kernel.c ./handlers/keyboard.o ./pit/pit.o ./syscall/syscall.o ./xagames/xagame_test.o  -o kernel.bin

cat ./xaninApps/shutdown ./xaninApps/syscall_test > ./xaninApps/xanin_external_apps

dd if=./xaninApps/xanin_external_apps of=./xaninApps/xanin_apps_space bs=512 count=16 conv=notrunc

cat ./boot/boot ./lib/enter_real_mode ./xaninApps/xanin_apps_space ./xin_fs/xin_pointers ./xin_fs/entries_table ./boot/kernelLoader kernel.bin > xanin.bin
#        0                   1                   2                   10                          14          15                                  


#MOV XANIN.img to BIN
dd if=xanin.bin of=xanin.img
mv xanin.img -f ../bin
rm xanin.bin 
rm kernel.bin
