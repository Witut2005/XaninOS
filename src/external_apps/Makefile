all:
	nasm -f elf32 write.asm -o write.o
	i386-elf-ld write.o -o write 

	nasm -f elf32 read.asm -o read.o
	i386-elf-ld read.o -o read 

	@# gcc -m32 -static puts.c -o puts

