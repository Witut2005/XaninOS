
#add 32-bit gcc cross-compiler to your $PATH variable


#ASSEMBLER COMPILATION
nasm -fbin ./boot/boot.asm 
nasm -fbin ./boot/locateFS.asm
nasm -fbin ./boot/kernelLoader.asm
nasm -f elf32 ./handlers/keyboard.asm -o keyboard.o
nasm -f elf32 ./pit/pit.asm -o pit.o

#COMPILE
i386-elf-gcc -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-unused-function kernel.c ./handlers/keyboard.o ./pit/pit.o -o kernel.bin
cat ./boot/boot ./boot/kernelLoader kernel.bin ./boot/locateFS files.tar > xanin.bin



#MOV XANIN.img to BIN

dd if=xanin.bin of=xanin.img
mv xanin.img -f ../bin
cd ../bin
make
./detectUstar
cd ../src

#CLEANING SRC
rm xanin.bin
rm pit.o
rm keyboard.o


