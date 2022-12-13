all:
	nasm -f elf32 write.asm -o write.o
	i386-elf-ld write.o -o write 

	nasm -f elf32 read.asm -o read.o
	i386-elf-ld read.o -o read 

	nasm -f elf32 reboot.asm -o reboot.o
	i386-elf-ld reboot.o -o reboot 


