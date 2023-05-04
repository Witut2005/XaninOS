all:
	nasm -f elf32 write.asm -o write.o
	i386-elf-ld write.o -o write 

	nasm -f elf32 read.asm -o read.o
	i386-elf-ld read.o -o read 

	nasm -f elf32 reboot.asm -o reboot.o
	i386-elf-ld reboot.o -o reboot 

	nasm -f elf32 link_create.asm -o link_create.o
	i386-elf-ld link_create.o -o link_create 

	i386-elf-gcc test.c -c
	i386-elf-gcc test.o -L /home/witut/Desktop/xaninOS/src/external_apps -o test -lgcc
	# i386-elf-gcc test.o -L ./libc.a -o test

